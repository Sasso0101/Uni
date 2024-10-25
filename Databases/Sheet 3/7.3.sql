-- Use the view again, but instead, for each country, return
-- - the name of the most profitable company
-- - how much it has spent
-- - the average amount of money spent in that country
-- - and how many companies are located in that country
select C1.country, company_name, company_max, avg_country, cnt_country
from CountriesWithCustomers C1
join (
  select country, avg(total) as avg_country, count(*) as cnt_country, max(total) as company_max
  from CountriesWithCustomers
  group by country
) C3 on C1.country = C3.country
where total = C3.company_max
order by total