UPDATE users SET cash = 10000 WHERE username = "bebe";

DELETE FROM portfolio WHERE user_id = (SELECT id FROM users WHERE username = "bebe");

DELETE FROM transactions WHERE user_id = (SELECT id FROM users WHERE username = "bebe");

SELECT * FROM users;
SELECT * FROM portfolio;
SELECT * FROM transactions;