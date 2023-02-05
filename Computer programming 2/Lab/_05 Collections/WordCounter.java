import java.io.*;
import java.util.*;
import java.util.Map.Entry;

public class WordCounter {
    private HashMap<String, Integer> wordMap = new HashMap<String, Integer>();

    private Comparator<Entry<String, Integer>> sortByValue = new Comparator<Entry<String, Integer>>() {
        @Override
        public int compare(Entry<String, Integer> o1, Entry<String, Integer> o2) {
            return o2.getValue().compareTo(o1.getValue());
        }
    };
    public void importFile(String filename) {
        try (
            FileReader file = new FileReader(filename);
            BufferedReader reader = new BufferedReader(file);
        ) {
            String line;
            while((line = reader.readLine()) != null) {
                if (line.contains("Inferno: Canto ")) continue;
                line = line.trim();
                String words[] = line.split("\\W"); //Break using non word characters
                for(String word: words) {
                    if (word.length() == 0) continue;
                    word = word.toLowerCase();
                    Integer value = wordMap.get(word);
                    if (value != null) wordMap.replace(word, value+1);
                    else wordMap.put(word, 1);
                }
            }
            Set<Entry<String, Integer>> wordSet = wordMap.entrySet();
            List<Entry<String, Integer>> wordList = new ArrayList<>(wordSet);
            Collections.sort(wordList, sortByValue);
            for (Map.Entry<String, Integer> word: wordList) {
                if (word.getValue() > 20) {
                    System.out.println(word.getKey()+": "+word.getValue());
                }
            }
        } catch (IOException ex) {
            ex.printStackTrace();
        }
    }

    public static void main(String args[]) {
        WordCounter wordCounter = new WordCounter();
        wordCounter.importFile("inferno.txt");
    }
}
