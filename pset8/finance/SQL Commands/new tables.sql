CREATE TABLE portfolio (user_id INTEGER NOT NULL,
                        symbol VARCHAR(5) NOT NULL UNIQUE,
                        shares INTEGER NOT NULL,
                        FOREIGN KEY (user_id) REFERENCES users(id));

CREATE TABLE transactions (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE,
                        user_id INTEGER NOT NULL,
                        symbol VARCHAR(5) NOT NULL,
                        price NUMERIC NOT NULL,
                        shares INTEGER NOT NULL,
                        transacted_on timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
                        UNIQUE(id),
                        FOREIGN KEY (user_id) REFERENCES users(id));