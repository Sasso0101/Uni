package data;

public enum Degree {
    bachelors,
    masters;

    public static Degree parse(String s) {
        if (s.equals("LT")) return Degree.bachelors;
        else if (s.equals("LM")) return Degree.masters;
        return null;
    }
}
