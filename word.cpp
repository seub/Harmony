#include "word.h"

Word::Word(const std::vector<letter> letters) : letters(letters)
{
}

Word Word::contract(const Word &w)
{
    if (w.size() == 0)
    {
        return w;
    }

    letter l, x;
    std::vector<letter> letters;
    int symbol = w[0].first;
    int power=0;

    unsigned int index =0;
    while(index != w.letters.size())
    {
        x = w.letters[index];
        if(x.first == symbol)
        {
            power += x.second;
            ++index;
        }
        else
        {
            if (power!=0)
            {
                l.first = symbol;
                l.second = power;
                letters.push_back(l);
                symbol = x.first;
                power = x.second;
                ++index;
            }
            else if(letters.size() !=0)
            {
                symbol = letters.back().first;
                power = letters.back().second;
                letters.pop_back();
            }
            else
            {
                symbol = x.first;
                power = x.second;
                ++index;
            }
        }
    }
    l.first = symbol;
    l.second = power;
    if (power != 0) letters.push_back(l);
    return Word(letters);
}

void Word::contract()
{
    *this = contract(*this);
}

Word operator*(Word w1, const Word &w2)
{
    w1.letters.insert(w1.letters.end(), w2.letters.begin(), w2.letters.end());
    return w1;
}

std::vector<Word> operator*(const Word &w, std::vector<Word> V)
{
    for(auto &x : V)
    {
        x = w*x;
    }
    return V;
}


std::vector<Word> operator*(std::vector<Word> V, const Word &w)
{
    for(auto &x : V)
    {
        x = x*w;
    }
    return V;
}

std::vector<Word> Word::contract(std::vector<Word> V)
{
    for(auto &w : V)
    {
        w.contract();
    }
    return V;
}

void Word::clear()
{
    letters.clear();
    return;
}

void Word::push_back(const letter &l)
{
    letters.push_back(l);
    return;
}

letter & Word::back()
{
    return letters.back();
}

void Word::reserve(std::vector<letter>::size_type N)
{
    letters.reserve(N);
    return;
}

std::vector<letter>::size_type Word::size() const
{
    return letters.size();
}

letter & Word::operator[](std::vector<letter>::size_type i)
{
    return letters[i];
}

letter Word::operator[](std::vector<letter>::size_type i) const
{
    return letters[i];
}

const std::vector<letter> & Word::getLetters() const
{
    return letters;
}


Word Word::inverse() const
{
    std::vector<letter>::size_type i, N=letters.size();
    std::vector<letter> inverseLetters(N);
    for (i=0; i!=N; ++i)
    {
        inverseLetters[N-1-i] = letter(letters[i].first, -letters[i].second);
    }
    return Word(inverseLetters);
}
