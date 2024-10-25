-- The previous report was liked by the Megadirector, but he would prefer to see those countries (between the 5th and 10th) listed in alphabetical order:
-- - modify the query to sort them appropriately,
-- - but add an additional column that shows the rank of each country, by amount of money spent, as before
select * from (
  select country, sum(total) as total_rev, rank() over (order by sum(total) desc)
  from CountriesWithCustomers
  group by country
  order by total_rev desc
  limit 6 offset 4
)
order by country