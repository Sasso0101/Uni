public class Student {
    private Career career;
    private String surname;
    private String name;

    public void setCareer(Career career) {
        this.career = career;
    }
    public Career getCareer() {
        return career;
    }
    public void setSurname(String surname) {
        this.surname = surname;
    }
    public String getSurname() {
        return surname;
    }
    public void setName(String name) {
        this.name = name;
    }
    public String getName() {
        return name;
    }
    public String toString() {
        return "Name: " + name + "\nSurname: " + surname + "\n" + career;
    }
    public void print() {
        System.out.println(this.toString());
    }
    public Student(String name, String surname) {
        this.name = name;
        this.surname = surname;
        career = new Career();
    }
    public static void main(String args[]) {
        Student s1 = new Student("Salvatore", "Andaloro");
        Career c1 = new Career();
        c1.newExam("Geometry", "30");
        c1.newExam("Programming 1", "28");
        c1.newExam("Calculus 1", "23");
        c1.newExam("Programming 2", "30L", "2023-2-14");
        s1.setCareer(c1);
        System.out.println(s1);
    }
}
