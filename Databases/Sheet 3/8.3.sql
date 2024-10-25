-- Now, either use the view we created or create a new one that includes the region for each country. Using this view,
-- - list the countries alphabetically (all of them now, no LIMIT),
-- - include the region, if any, for each country,
-- - modify the ranking to be within each country’s regions instead of globally
-- Ignore companies: only consider the countries, their regions, and the total amount spent
with countries_report as (
  select country, region, sum(unit_price*quantity*(1-discount)) as total
  from orders O
  join order_details OD on OD.order_id = O.order_id
  join customers C on O.customer_id = C.customer_id
  group by country, region
)
select *, rank() over (
  partition by country
  order by total desc
)
from countries_report
order by country asc