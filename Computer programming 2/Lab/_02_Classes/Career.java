import java.util.Date;

import java.text.DateFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;

public class Career {
    protected int numExams = 0;
    protected double mean = 0;
    protected static final int MAX_EXAMS = 10;
    protected Exam exams[] = new Exam[MAX_EXAMS];

    public String toString() {
        String output = "Number of exams: " + numExams + "\nMean: " + mean;
        for (int i = 0; i < numExams; i++) {
            output += "\n" + exams[i];
        }
        return output;
    }
    public void print() {
        System.out.println(this.toString());
    }
    private Date parseDate(String date) throws ParseException {
        DateFormat format = new SimpleDateFormat("yyyy-MM-dd");
        return format.parse(date);
    }
    public void newExam(String subject, String mark, Date date) {
        if (numExams == MAX_EXAMS) {
            System.out.println("TOO MANY EXAMS!");
            return;
        } else {
            exams[numExams] = new Exam(subject, mark, date);
            numExams++;
            updateMean();
        }
    }
    public void newExam(String subject, String mark, String date) {
        try {
            this.newExam(subject, mark, parseDate(date));
        } catch (ParseException e) {
            e.printStackTrace();
        }
    }
    public void newExam(String subject, String mark) {
        this.newExam(subject, mark, new Date());
    }
    private void updateMean() {
        int sum = 0;
        for (int i = 0; i < numExams; i++) {
            sum += exams[i].getMark().getMarkNoLode();
        }
        this.mean = (double)sum/numExams;
    }
    public static void main(String args[]) {
        Career c1 = new Career();
        c1.newExam("Geometry", "30");
        c1.newExam("Programming 1", "28");
        c1.newExam("Calculus 1", "23");
        System.out.println(c1);
    }
}
