import nltk

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):

        self.positive_list = []
        self.negative_list = []

        # Read positive words into list
        with open(positives) as file:
            for line in file:
                if line[0] is ';' or line[0] is '\n':
                    continue
                else:
                    line = line.rstrip("\n")
                    self.positive_list.append(line)

        # Read negative words into list
        with open(negatives) as file:
            for line in file:

                if line[0] is ';' or line[0] is '\n':
                    continue
                else:
                    line = line.rstrip("\n")
                    self.negative_list.append(line)


    def analyze(self, text):

        #word_list = text.split()
        tokenizer = nltk.tokenize.TweetTokenizer()
        word_list = tokenizer.tokenize(text)

        score = 0

        for word in word_list:
            word = word.lower()
            if word in self.positive_list:
                score += 1
            elif word in self.negative_list:
                score -= 1

        return score

