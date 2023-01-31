import java.text.SimpleDateFormat;
import java.util.Date;

public class Exam {
    private Mark mark;
    private Date date;
    private String subject;

    public void setMark(String mark) {
        this.mark = new Mark(mark);
    }
    public Mark getMark() {
        return mark;
    }
    public void setDate(Date date) {
        this.date = date;
    }
    public Date getDate() {
        return date;
    }
    public void setSubject(String subject) {
        this.subject = subject;
    }
    public String getSubject() {
        return subject;
    }
    public String toString() {
        SimpleDateFormat dateFormat = new SimpleDateFormat("dd/MM/yyyy");
        return "Subject: " + subject + " Mark: " + mark + " Date: " + dateFormat.format(date);
    }
    public void print() {
        System.out.println(this.toString());
    }
    public Exam(String subject, String mark, Date date) {
        this.subject = subject;
        this.date = date;
        this.mark = new Mark(mark);
    }
    public Exam(String subject, String mark) {
        this.subject = subject;
        this.date = new Date();
        this.mark = new Mark(mark);
    }
    public static void main(String args[]) {
        Exam e1 = new Exam("Calculus 1", "30");
        System.out.println(e1);
    }
}
