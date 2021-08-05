SELECT title FROM movies JOIN stars ON movies.id = stars.movie_id JOIN people
ON stars.person_id = people.id WHERE name = 'Johnny Depp'
INTERSECT
SELECT title FROM movies JOIN stars ON movies.id = stars.movie_id JOIN people
ON stars.person_id = people.id WHERE name = 'Helena Bonham Carter';

-- SELECT title FROM movies JOIN stars ON movies.id = stars.movie_id JOIN people ON stars.person_id = people.id
-- WHERE name IN ('Johnny Depp', 'Helena Bonham Carter')
-- GROUP BY movie_id
-- HAVING COUNT(DISTINCT person_id) = 2;