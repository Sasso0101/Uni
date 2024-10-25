-- Using the newly created view, list all the company names that have spent more than the average in their respective countries. Return the same columns as in the view, but sort them by the amount of money spent
select country, company_name, order_cnt, total
from CountriesWithCustomers C1
where total > (
  select avg(total)
  from CountriesWithCustomers C2
  where C1.country = C2.country 
  group by country
)
order by total