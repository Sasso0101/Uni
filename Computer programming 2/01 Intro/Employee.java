class Employee {
    private String name;
    private String surname;
    private double salary;

    public Employee(String name, String surname, double salary) {
        this.name = name;
        this.surname = surname;
        this.salary = salary;
    }

    public void rename(String newName) {
        name = newName;
    }
    public void changeSalary(double newSalary) {
        salary = newSalary;
    }
    public void increaseSalary(double increasePercentage) {
        System.out.println((100+increasePercentage)/100);
        salary = salary*((100+increasePercentage)/100);
    }

    public void print() {
        System.out.println("Name: "+name);
        System.out.println("Surname: "+surname);
        System.out.println("Salary: "+salary+"€");
    }

    public static void main(String args[]) {
        Employee e1 = new Employee("Will", "Smith", 1000);
        e1.increaseSalary(50);
        e1.rename("Willy");
        e1.print();
    }
}