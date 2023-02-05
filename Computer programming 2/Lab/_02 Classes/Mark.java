public class Mark {
    int mark;
    Mark(String mark) {
        if (mark == "30L") this.mark = 31;
        else this.mark = Integer.parseInt(mark);
    }
    Mark(int mark, boolean lode) {
        if (lode) this.mark = 31;
        else this.mark = mark;
    }
    Mark(int mark) {
        this.mark = mark;
    }
    public String toString() {
        if (mark == 31) return "30L";
        else return Integer.toString(mark);
    }
    public int getMarkNoLode() {
        if (mark == 31) return 30;
        else return mark;
    }
}
