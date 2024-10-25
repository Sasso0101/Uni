-- A simple SELECT * FROM Employees may not show that immediately, but the employees in Northwind are organized in a hierarchy. List all the employees that
-- - are not managers,
-- - show their Name, Surname, and Name and Surname of their manager (if they have one),
-- - and sort them by their manager’s surname
select E.first_name as employee_first_name, E.last_name as employee_last_name, MAN.first_name as manager_first_name, MAN.last_name as manager_last_name
from employees E
join employees MAN on E.reports_to = MAN.employee_id
where E.employee_id not in (
  select reports_to
  from employees
  where reports_to is not null
)
order by manager_last_name asc