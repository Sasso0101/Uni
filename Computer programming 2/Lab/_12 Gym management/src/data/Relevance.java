package data;

public enum Relevance {
    national,
    international;

    public static Relevance parse(String s) {
        if (s.equals("national")) return Relevance.national;
        else if (s.equals("international")) return Relevance.international;
        return null;
    }
}