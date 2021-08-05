import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # TODO: Sort stocks by alphabetical order

    # Get user's cash balance
    cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]
    cash = round(cash, 2)  # Round to two decimal places (check this if calculations broken)
    balance = cash

    # Get user's portfolio
    portfolio = db.execute("SELECT symbol, shares FROM portfolio WHERE user_id = ? ORDER BY symbol", session["user_id"])

    # Lookup the current price and name of each stock in the user's portfolio's
    for stock in portfolio:
        stock_info = lookup(stock["symbol"])
        stock["name"] = stock_info["name"]
        stock["price"] = usd(stock_info["price"])
        stock["total"] = stock_info["price"] * stock["shares"]
        balance += stock["total"]
        stock["total"] = usd(stock["total"])

    return render_template("index.html", cash=usd(cash), portfolio=portfolio, balance=usd(balance))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # TODO: Add an alert on index page after buying a stock

    # User reached route via POST (as by submitting a form via POST)
    # Purchase the stock so long as user can afford
    if request.method == "POST":

        # Lookup the user's given symbol using the API
        quote_info = lookup(request.form.get("symbol"))

        # Ensure user submitted a symbol
        if not request.form.get("symbol"):
            return apology("missing symbol", 400)

        # Ensure submited symbol is valid
        elif quote_info is None:
            return apology("invalid symbol", 400)

        # Ensure user did not submit non-numeric characters and submitted only integers
        elif not request.form.get("shares").isdigit():
            return apology("only include positive integers", 400)

        # Ensure user submitted a positive integer for number of shares
        elif not int(request.form.get("shares")) >= 1:
            return apology("minimum number of shares is 1", 400)

        else:

            # Get user's cash balance
            cash_query = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
            new_balance = cash_query[0]["cash"] - quote_info["price"] * int(request.form.get("shares"))

            # Ensure user has enough cash to buy the requested stocks
            if new_balance < 0:
                return apology("Insufficient cash", 400)

            else:

                # Insert purchase into transactions table
                db.execute("INSERT INTO transactions (user_id, symbol, price, shares) VALUES (?, ?, ?, ?)",
                            session["user_id"], quote_info["symbol"], usd(quote_info["price"]), request.form.get("shares"))

                # Check stock in portfolio
                stock_query = db.execute("SELECT shares FROM portfolio WHERE user_id = ? AND symbol = ?",
                                         session["user_id"], quote_info["symbol"])

                # User never had the stock before
                if len(stock_query) == 0:
                    db.execute("INSERT INTO portfolio (user_id, symbol, shares) VALUES (?, ?, ?)",
                                session["user_id"], quote_info["symbol"], request.form.get("shares"))

                else:  # Stock exists in user's portfolio (can be 0 shares)
                    db.execute("UPDATE portfolio SET shares = ? WHERE user_id = ? AND symbol = ?",
                            stock_query[0]["shares"] + int(request.form.get("shares")),
                            session["user_id"], quote_info["symbol"])

                # Deduct cost of the stocks from user's cash
                db.execute("UPDATE users SET cash = ? WHERE id = ?",
                            new_balance, session["user_id"])

            # Redirect user to home page
            flash("Bought!")
            return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    # Display form to buy a stock
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    transactions = db.execute("SELECT symbol, price, shares, transacted_on FROM transactions \
                               WHERE user_id = ? ORDER BY transacted_on",
                               session["user_id"])

    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        flash("Login successful!")
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Lookup the user's given symbol using the API
        quote_info = lookup(request.form.get("symbol"))

        # Ensure user submitted a symbol
        if not request.form.get("symbol"):
            return apology("missing symbol", 400)

        # Ensure submited symbol is valid
        elif quote_info is None:
            return apology("invalid symbol", 400)

        else:
            return render_template("quoted.html", symbol=quote_info["symbol"],
                company=quote_info["name"], price=quote_info["price"])

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure username does not already exist
        elif len(db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))) != 0:
            return apology("username already exists", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Ensure password and confirmation matches
        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("passwords do not match", 403)

        # Insert the new user into users table
        db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)",
            username=request.form.get("username"),
            hash=generate_password_hash(request.form.get("password")))

        # Redirect user to home page
        flash("Registration successful. Please login.")
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # TODO: Add an alert when user succesfully sells a stock on the index page

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure user submitted a stock
        if request.form.get("symbol") is None:
            return apology("missing symbol", 400)

        # Ensure user did not submit non-numeric characters and submitted only integers
        elif not request.form.get("shares").isdigit():
            return apology("only include positive integers", 400)

        stock = db.execute("SELECT shares FROM portfolio WHERE user_id = ? AND symbol = ?",
                                session["user_id"], request.form.get("symbol"))

        # Ensure user owns atleast one stock
        if len(stock) < 1 or stock[0]["shares"] < 1:
            return apology("you do not own any share of the requested stock", 400)

        # Ensure user requests to sell a positive integer of stocks
        elif int(request.form.get("shares")) < 1:
            return apology("can only sell positive number of stocks", 400)

        # Ensure user has enough stocks to be sold
        elif int(request.form.get("shares")) > stock[0]["shares"]:
            return apology("requested shares is more than currently owned", 400)

        else:

            # Lookup the user's given symbol using the API
            quote_info = lookup(request.form.get("symbol"))

            # Get user's cash balance
            cash_query = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
            new_balance = cash_query[0]["cash"] + quote_info["price"] * int(request.form.get("shares"))

            # Insert sale into transactions table
            db.execute("INSERT INTO transactions (user_id, symbol, price, shares) VALUES (?, ?, ?, ?)",
                session["user_id"], quote_info["symbol"], usd(quote_info["price"]), "-" + request.form.get("shares"))

            # If user requests to sell all the shares he/she has
            if int(request.form.get("shares")) == int(stock[0]["shares"]):
                db.execute("DELETE FROM portfolio WHERE user_id = ? AND symbol = ?",
                            session["user_id"], quote_info["symbol"])

            # User requested to sell less stocks than he/she has
            else:

                db.execute("UPDATE portfolio SET shares = ? WHERE user_id = ? AND symbol = ?",
                            stock[0]["shares"] - int(request.form.get("shares")),
                            session["user_id"], quote_info["symbol"])

            # Update the user's cash balance to reflect the profit from the sold shares
            db.execute("UPDATE users SET cash = ? WHERE id = ?",
                        new_balance, session["user_id"])

            # Redirect user to home page
            flash("Sold!")
            return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:

        portfolio = db.execute("SELECT symbol, shares FROM portfolio WHERE user_id = ?", session["user_id"])
        return render_template("sell.html", portfolio=portfolio)


@app.route("/changepw", methods=["GET", "POST"])
@login_required
def changepw():
    """Allow user to change password"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure user submitted all fields
        if not request.form.get("current_pw"):
            return apology("Missing current password", 400)

        elif not request.form.get("new_pw"):
            return apology("Missing new password", 400)

        elif not request.form.get("confirm_pw"):
            return apology("Missing password confirmation", 400)

        # Ensure new password matches the password confirmation
        elif (request.form.get("new_pw") != request.form.get("confirm_pw")):
            return apology("New password does not match password confirmation", 400)

        # Query username table for hashed password
        hash_query = db.execute("SELECT hash FROM users WHERE id = ?", session["user_id"])[0]

        # Ensure submitted current_password hash matches the password in the database
        if not check_password_hash(hash_query["hash"], request.form.get("current_pw")):
            return apology("Submitted password is incorrect", 400)

        else:
            # Update hashed password in the database to the new one
            db.execute("UPDATE users SET hash = :hash WHERE id = :user_id",
                        hash=generate_password_hash(request.form.get("new_pw")), user_id=session["user_id"])

        # Redirect user to home page
        flash("Password changed succesfully!")
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("changepw.html")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
