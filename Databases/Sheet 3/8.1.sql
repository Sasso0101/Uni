-- The Megadirector has an attention span of a goldfish and finds your report too verbose.
-- Use the view we created to create a query that
-- - has countries ordered by the amount of money spent
-- - lists only the countries and their revenues
-- - prints only countries between the 5th and 10th in the list (both inclusive)
select country, sum(total) as total_rev
from CountriesWithCustomers
group by country
order by total_rev desc
limit 6 offset 4