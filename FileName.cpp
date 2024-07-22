#include <iostream>
#include <concepts>
#include <vector>
#include <string>
#include <Windows.h>
#include "header.h"

using namespace std;

class Reader;
enum genre { Драма, Детектив, Поэзия, Роман, Повесть, Рассказ, Сказка, Басня };

struct Author
{
private:
    string fio;
    string country;
public:
    Author(string f, string c) : fio(f), country(c) {}

    string autor_fio()
    {
        return fio;
    }
};

class Book
{
private:
    string name;
    Author writer;
    int year;
    int number;
    genre Bookgenre;

public:

    Book(string n, Author w, int y, int nu, genre bookgenre) : name(n), writer(w), year(y), number(nu), Bookgenre(bookgenre) {}

    bool operator==(Book other)
    {
        return (this->book_name() == other.book_name() && this->book_year() == other.book_year());
    }

    bool operator!=(Book book)
    {
        return !(*this == book);
    }
    string book_name()
    {
        return name;
    }

    int book_year()
    {
        return year;
    }

};

class People
{
private:
    string fio;
    int age;
public:
    People(string f, int n) : fio(f), age(n) {}
    virtual void giveBook(Book& book) {}
    virtual void takeBook(Book& book) {}


    string people_fio()
    {
        return fio;
    }
    int people_age()
    {
        return age;
    }

};

class Library;

class Librarian : public People
{
private:
    Librarian() : People("Watch_dog", 66) {}
    // Librarian(const Librarian&);
    // Librarian& operator= (Librarian&);
    static Librarian* instance_ptr;

public:
    void takeBook(Book& book) override
    {
        cout << "Librarian " << people_fio() << " received book " << book.book_name() << endl;
    }

    void giveBook(Book& book) override
    {

        cout << "Librarian looking for book " << book.book_name() << endl;
    }

    ~Librarian() { instance_ptr = nullptr; }

    static Librarian* get_instance()
    {
        if (instance_ptr == nullptr)
            instance_ptr = new Librarian();
        return instance_ptr;
    }
};

Librarian* Librarian::instance_ptr = nullptr;

class Library
{

private:
    int countBook(vector<Book> arr, Book book)
    {
        int count = 0;
        for (int i = 0; i < arr.size(); i++)
        {
            if (arr[i] == book)
                count++;
        }
        return count;
    }

    string name;
    string address;
    vector<Book> books;
    vector<Reader> readers;
    Librarian* librarian;

public:

    Library(string n, string a, Librarian* l) : name(n), address(a), librarian(l) {}

    void addBook(Book book)
    {
        books.push_back(book);
        cout << "book " << book.book_name() << " was added in library" << endl;
    }

    void dellBook(Book book)
    {
        if (available(book) == 1)
        {
            books.erase(remove(books.begin(), books.end(), book), books.end());
            cout << "book " << book.book_name() << " was taken from library" << endl;
        }
        else
        {
            //  cout << " no " << book.book_name() << endl;
            return;
        }

    }

    bool available(Book book)
    {
        if (countBook(books, book) > 0)
            return 1;
        else
            return 0;
    }

    Library& operator--()
    {
        SYSTEMTIME st;
        GetLocalTime(&st);
        for (auto it = books.begin(); it != books.end();)
        {
            if (st.wYear - it->book_year() > 5)
            {
                cout << "Списывается книга " << it->book_name() << ", posted in " << it->book_year() << endl;
                it = books.erase(it);
            }
            else
                ++it;
        }
        return *this;
    }
};

class Reader : public People
{
private:
    vector<Book> borrowedBooks;
    Librarian* librarian;
    Library* library;
    int number;
    MyList<int, 5> mylist;

public:
    Reader(string f, int n, Librarian* lib, Library* libb, int num) : People(f, n), librarian(lib), library(libb), number(num), mylist(mylist) {}

    void takeBook(Book& book) override
    {
        if (library->available(book))
        {
            borrowedBooks.push_back(book);
            mylist.add(number);
            cout << "reader " << people_fio() << " got book " << book.book_name() << endl;
        }
        else
        {
            cout << "reader didnt get " << book.book_name() << endl;
            return;
        }
    }

    void giveBook(Book& book) override
    {
        borrowedBooks.erase(remove(borrowedBooks.begin(), borrowedBooks.end(), book), borrowedBooks.end());
        mylist.del();
        cout << "reader " << people_fio() << " gave back book " << book.book_name() << endl;
    }
};


class Facad
{
private:
    Library* library;
    Librarian* librarian;
    Reader* reader;
public:
    Facad(Library* l, Librarian* li, Reader* r) : library(l), librarian(li), reader(r) {}
    void book_away(Book& book)
    {
        librarian->giveBook(book);
        reader->takeBook(book);
        library->dellBook(book);
        cout << endl;
    }

    void book_back(Book& book)
    {
        reader->giveBook(book);
        librarian->takeBook(book);
        library->addBook(book);
        cout << endl;
    }
};


int main()
{
    Author a("A A A", "RUS");
    Book b("book1", a, 2015, 1, genre::Драма);
    Book b1("book2", a, 2015, 1, genre::Драма);
    Book b3("book3", a, 2015, 1, genre::Драма);
    Book b4("book4", a, 2015, 1, genre::Драма);
    Book b5("book5", a, 2015, 1, genre::Драма);
    Book b7("book7", a, 2015, 1, genre::Драма);

    Librarian* l = Librarian::get_instance();
    Librarian* l1 = Librarian::get_instance();
    cout << l << endl;
    cout << l1 << endl;

    Library lib("Library #1", "zona 14", l);
    Reader r("D D D", 18, l, &lib, 15);
    Facad sys(&lib, l1, &r);
    lib.addBook(b); lib.addBook(b1); lib.addBook(b3); lib.addBook(b4); lib.addBook(b5); lib.addBook(b7);
    sys.book_away(b);
    sys.book_away(b1);
    sys.book_away(b5);
    sys.book_away(b3);
    sys.book_away(b4);
    sys.book_away(b7);

}
