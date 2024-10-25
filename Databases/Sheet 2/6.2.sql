-- Northwind Traders’ Megadirector is considering for promotion one or more employees that have shown exceptional performance and are not managers yet. Find each employee that
-- (1) has shipped the most profitable order ever
-- (2) has the highest average profit per order
-- and list their Name, Surname, and what they have achieved. The query must work even if there are ties
with not_managers as (
  select E.employee_id
  from employees E
  join employees MAN on E.reports_to = MAN.employee_id
  where E.employee_id not in (
    select reports_to
    from employees
    where reports_to is not null
  )
),
employee_orders as (
  select O.order_id, E.employee_id, sum(unit_price*quantity*(1-discount)) as profit
  from not_managers E
  join orders O on O.employee_id = E.employee_id
  natural join order_details OD
  group by O.order_id, E.employee_id
),
employee_avg as (
  select employee_id, avg(profit) as avg_profit
  from employee_orders
  group by employee_id
)
select employee_id, 'shipped most profitable order'
from employee_orders
where profit = (
  select max(profit)
  from employee_orders
)
group by employee_id, profit
union
select employee_id, 'has highest average profit per order'
from employee_avg
where avg_profit = (
  select max(avg_profit)
  from employee_avg
)