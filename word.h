#ifndef WORD_H
#define WORD_H

#include "tools.h"


class Word
{
    friend Word operator*(Word w1, const Word &w2);
    friend std::vector<Word> operator*(const Word &w, std::vector<Word> V);
    friend std::vector<Word> operator*(std::vector<Word> V, const Word &w);
    friend std::ostream & operator <<(std::ostream &out, const Word &w);

public:
    Word(const std::vector<letter> letters = {});

    letter & operator[](std::vector<letter>::size_type i);
    letter operator[](std::vector<letter>::size_type i) const;
    void clear();
    void push_back(const letter &l);
    letter & back();
    void reserve(std::vector<letter>::size_type N);
    std::vector<letter>::size_type size() const;

    const std::vector<letter> & getLetters() const;

    static Word contract(const Word &w);
    static std::vector<Word> contract(std::vector<Word> V);
    void contract();
    Word inverse() const;

private:
    std::vector<letter> letters;
};

#endif // WORD_H
