SELECT name FROM people WHERE people.id IN (SELECT person_id FROM stars JOIN movies
ON stars.movie_id = movies.id WHERE title = 'Toy Story');