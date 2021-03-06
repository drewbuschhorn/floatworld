#include <assert.h>
#include <typeinfo>
#include <string.h>
#include <sstream>

#include <QObject>
#include <QWidget>

#include "../gui/widgets.hpp"
#include "metaclass.hpp"
#include "misc.hpp"

bool human_readable = true;

using namespace std;

RegisterAbstractClass(Object, None);
RegisterVar(Object, rng);

std::istream& operator>>(std::istream& s, QString& str)
{
    char c;

    s.get(c);
    assert(c == '\"');

    bool escaped = false;
    do {
        s.get(c);
        if (escaped)
        {
            if (c == '"') str.append('"');
            else if (c == '\\') str.append('\\');
            else { str.append('\\'); str.append(c); }
            escaped = false;
        } else {
            if (c == '"') break;
            else if (c == '\\') escaped = true;
            else str.append(c);
        }
    } while(true);

    return s;
}

std::ostream& operator<<(std::ostream& os, QString& s)
{
    QByteArray bytes = s.toAscii();
    int len = bytes.length();

    os << "\"";
    for(int i = 0; i < len; i++)
    {
        char c = bytes.at(i);
        if (c == '\\') os << "\\\\";
        else if (c == '\"') os << "\\\"";
        else os << c;
    }
    os << "\"";
    return os;
}

std::istream& operator>>(std::istream& is, const char* str)
{
    if (*str == '\0')
    {
        char ch;
        do is.get(ch); while (ch && isspace(ch));
        is.putback(ch);
        return is;
    }

    const char* s = str;
    char ch;

    do {
        is.get(ch);
        if (ch == 0)
        {
            cout << "Early termination with null symbol" << endl;
            cout << "Was expecting <" << str << ">" << endl;
            throw "error";
        }
        if (isspace(ch) && isspace(*s))
        {
            while (isspace(*s)) { s++; if (*s == 0) goto done; }
            while (isspace(ch)) is.get(ch);
        }
        if (ch != *s)
        {
            cout << "Expectation failed!" << endl;
            cout << "Was trying to match:" << endl;
            cout << "<" << str << ">" << endl;
            cout << "Got up to:" << endl;
            cout << "<" << s << ">" << endl;
            cout << "Last character read: <" << ch << ">" << endl;
            cout << "Rest of stream:" << endl;
            char s[128];
            is.read(s, 128);
            cout << s;
            throw "error";
        }
        s++;
    } while (*s);
    done:
    //cout << "matched <" << str << ">" << endl;
    return is;
};

std::string get_word(std::istream& is)
{
    char ch;
    string str;
    do {
        is.get(ch);
        if (isalnum(ch)) str += ch;
        else
        {
            is.putback(ch);
            break;
        }
    } while (ch);
    return str;
}

std::ostream& operator<<(std::ostream& os, Object& c)
{
    if (human_readable) os << "{" << endl;
    if (human_readable) os << "\t\"class\": \"";
    os << c.Name();
    if (human_readable) os << "\",";
    os << endl;
    c.Write(os);
    if (human_readable) os << "}" << endl;
    return os;
}

std::istream& operator>>(std::istream& is, Object& c)
{
    const char* name = c.Name();
    if (human_readable) is >> "{\t\"class\": \"";
    is >> name;
    if (human_readable) is >> "\",";
    c.Read(is);
    return is;
}

int Class::nmetaclasses = 0;
Class* Class::metaclasses[128];

Object::Object()
{
    panel = NULL;
}

Object::~Object()
{
}

const char* Object::Name()
{
    const char* name = typeid(*this).name();
    while (*name >= '0' and *name <= '9') name++;
    return name;
}

void Object::Reset()
{
}

BindingsPanel* Object::SetupPanel(bool title)
{
    if (panel) return panel;
    panel = new BindingsPanel(&GetClass(), this);
    if (title) panel->CreateTitle();
    panel->ConstructChildren();
    panel->UpdateChildren();
    return panel;
}

void Object::UpdatePanel()
{
    if (panel) panel->UpdateChildren();
}

void Object::DeletePanel()
{
    if (!panel) return;
    delete panel;
    panel = NULL;
}

void Object::HookWasChanged()
{
}

Class& Object::GetClass()
{
    return *Class::Lookup(Name());
}

void Object::Write(ostream& os)
{
    if (human_readable)
    {
        stringstream ostr;
        string line;
        GetClass().Write(this, ostr);
        while(std::getline(ostr, line))
        {
            os << "\t" << line << endl;
        }
    } else {
        GetClass().Write(this, os);
    }
}

void Object::Read(istream& is)
{
    GetClass().Read(this, is);
}

Class::Class(const char* _name, const char* _pname, ObjectMaker func)
    : name(_name),
      pname(_pname),
      maker(func),
      nvars(0),
      nqvars(0)
{
    abstract = (func == NULL);
    metaclasses[nmetaclasses] = this;
    nmetaclasses++;
}

Class* Class::Lookup(const char* name)
{
    for (int i = 0; i < nmetaclasses; i++)
    {
        if (strcmp(name, metaclasses[i]->name) == 0) return metaclasses[i];
    }
    return NULL;
}

Object* Class::MakeNew(const char* name)
{
    Class* metaclass = Lookup(name);
    if (!metaclass)
    {
        cerr << "Couldn't find class \"" << name << "\"!" << endl;
        assert(metaclass);
    }

    return (*metaclass->maker)();
}

Object* Class::Create(std::istream& is)
{
//    cout << "Trying to read class" << endl;
    if (human_readable) is >> "{\t\"class\": \"";
    is >> whitespace;
    string name = get_word(is);
    if (human_readable) is >> "\"" >> whitespace >> ",";
    is >> whitespace;
//    cout << "Read the following classname: " << name << endl;
    Object* serial = MakeNew(name.c_str());
    serial->Read(is);
    is >> whitespace;
    if (human_readable) is >> "}";
    return serial;
}

void Class::Read(Object* c, istream& is)
{
//    cout << "Starting to read class: " << name << endl;
    Class* parent = Lookup(pname);
    if (parent)
    {
        parent->Read(c, is);
        if (human_readable) is >> whitespace >> ",";
        is >> whitespace;
    } else c->Reset();
    for (int i = 0; i < nvars; i++)
    {
//        cout << "Reading \"" << varname[i] << "\", var " << i << " of " << c->Name() << endl;
        if (human_readable) {
            if (i != 0) is >> whitespace >> "," >> whitespace;
            is >> whitespace >> "\"";
            is >> varname[i];
            is >> "\":" >> whitespace;
        } else is >> whitespace;
        (*readers[i])(c, is);
    }
//    cout << "Done reading class: " << name << endl;
}

void Class::Write(Object* c, ostream& os)
{
    Class* parent = Lookup(pname);
    if (parent)
    {
        parent->Write(c, os);
        if (human_readable) os << ", \n"; else os << endl;
    }
    for (int i = 0; i < nvars; i++)
    {
        if (human_readable) {
            if (i != 0) os << ",\n";
            os << '"' << varname[i] << "\": ";
        } else os << endl;
        (*writers[i])(c, os);
    }
}

Registrator::Registrator(Class& metaclass, const char* name, ObjectReader read, ObjectWriter write)
{
    metaclass.writers[metaclass.nvars] = write;
    metaclass.readers[metaclass.nvars] = read;
    metaclass.varname[metaclass.nvars] = name;
    metaclass.nvars++;
}

Registrator::Registrator(Class& metaclass, const char* label, HookFactory factory)
{
    int n = metaclass.nqvars++;
    metaclass.labels[n] = label;
    metaclass.factories[n] = factory;
}

