# Sentiment-Analysis-in-C
A C program that analyses text and determines the sentimenet and opinions expressed within it. 
## Table of Contents

- [About the Project](#about-the-project)
- [Getting Started](#getting-started)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)
- [Contact](#contact)

## About the Project

VADER (Valence Aware Dictionary and Sentiment Reasoner)  is a lexicon and rule-based sentiment
analysis tool specifically designed for analyzing the sentiment of text. It consists of a list of words with corresponding sentiment scores that are meant to indicate the potential positivity, negativity and neutrality of each word. The lexicon, vader_lexicon.txt,  was downloaded straight from the original repository by cjhutto.  More details about the implementation can be found in the report pdf within this repository.

## Getting Started

### Prerequisites

Make sure you have a C compiler like `gcc` installed. You can verify this by running:

```bash
gcc --version

# usage
first you need to run the `make` command in the terminal to build the files and the project structure.
After that, you can compile and run the code using this template:

./mySA vader lexicon.txt <other inputs> , where mySA is the executable object file created by your Makefile. This is assuming we don't have any other inputs. This just reads the vader lexicon, and using dynamic memory allocation, puts each word in the file and stores it within a struct that follows this template;

struct words {
char * word ;
float score ;
float SD ;
int SIS_array [10];
};

you can also run:

./mySA vader_lexicon.txt validation.txt, where mySA is the executable object file created by your Makefile. validation.txt is a seperate txt file that contains various sentences. When it is included as an argument, the code will read through each line of the file and return the average sentiment score for each sentence.

# Contributing
Contributions are welcome! If you find any issues or have suggestions for improvements, feel free to fork the repository, create a pull request, or open an issue.

# License

Distributed under the MIT License. See LICENSE for more information.

# Contact
Takunda Christian Takaindisa - christiantakaindisa@outlook.com
