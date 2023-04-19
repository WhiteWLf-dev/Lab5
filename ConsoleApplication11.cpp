#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

class TextComponent {
public:
    virtual string getText(const std::string& filename) = 0;
};

class PlainText : public TextComponent {
public:
    string getText(const std::string& filename) override {
        std::ifstream file(filename);
        if (file) {
            std::string content((std::istreambuf_iterator<char>(file)),
                (std::istreambuf_iterator<char>()));
            return content;
        }
        else {
            std::cerr << "Error opening file: " << filename << std::endl;
            return "";
        }
    }
};

class TextDecorator : public TextComponent {
protected:
    TextComponent* component_;
public:
    TextDecorator(TextComponent* component) : component_(component) {}

    string getText(const std::string& filename) override {
        return component_->getText(filename);
    }
};

class LogoDecorator : public TextDecorator {
public:
    LogoDecorator(TextComponent* component) : TextDecorator(component) {}
    string getText(const std::string& filename) override {
        string logo = R"(
                               /T /I
                              / |/ | .-~/
                          T\ Y  I  |/  /  _
         /T               | \I  |  I  Y.-~/
        I l   /I       T\ |  |  l  |  T  /
     T\ |  \ Y l  /T   | \I  l   \ `  l Y
 __  | \l   \l  \I l __l  l   \   `  _. |
 \ ~-l  `\   `\  \  \\ ~\  \   `. .-~   |
  \   ~-. "-.  `  \  ^._ ^. "-.  /  \   |
.--~-._  ~-  `  _  ~-_.-"-." ._ /._ ." ./
 >--.  ~-.   ._  ~>-"    "\\   7   7   ]
^.___~"--._    ~-{  .-~ .  `\ Y . /    |
 <__ ~"-.  ~       /_/   \   \I  Y   : |
   ^-.__           ~(_/   \   >._:   | l______
       ^--.,___.-~"  /_/   !  `-.~"--l_ /     ~"-.
              (_/ .  ~(   /'     "~"--,Y   -=b-. _)
               (_/ .  \  :           / l      c"~o \
                \ /    `.    .     .^   \_.-~"~--.  )
                 (_/ .   `  /     /       !       )/
                  / / _.   '.   .':      /        '
                  ~(_/ .   /    _  `  .-<_  
                    /_/ . ' .-~" `.  / \  \          ,z=.
                    ~( /   '  :   | K   "-.~-.______//
                      "-,.    l   I/ \_    __{--->._(==.
                       //(     \  <    ~"~"     //
                      /' /\     \  \     ,v=.  ((
                    .^. / /\     "  }__ //===-  `
                   / / ' '  "-.,__ {---(==-
                 .^ '       :  T  ~"   ll       
                / .  .  . : | :!        \\
               (_/  /   | | j-"          ~^
                 ~-<_(_.^-~"
)";

        string text = component_->getText(filename);
        text = logo + "\n" + text;
        return text;
    }
};

class BorderDecorator : public TextDecorator {
public:
    BorderDecorator(TextComponent* component) : TextDecorator(component) {}
    string getText(const std::string& filename) override {
        string str = component_->getText(filename);
        istringstream iss(str);
        vector<std::string> lines;
        string line;
        while (std::getline(iss, line)) {
            lines.push_back(line);
        }
        size_t max_length = 0;
        for (const auto& line : lines) {
            if (line.size() > max_length) {
                max_length = line.size();
            }
        }
        string framed_content = "+";
        for (size_t i = 0; i < max_length + 2; i++) {
            framed_content += "-";
        }
        framed_content += "+\n";
        for (const auto& line : lines) {
            framed_content += "| " + line;
            for (size_t i = line.size(); i < max_length; i++) {
                framed_content += " ";
            }
            framed_content += " |\n";
        }
        framed_content += "+";
        for (size_t i = 0; i < max_length + 2; i++) {
            framed_content += "-";
        }
        framed_content += "+\n";
        return framed_content;
    }
};

class CompanyLicenseDecorator : public TextDecorator {
public:
    CompanyLicenseDecorator(TextComponent* component) : TextDecorator(component) {}
    string getText(const std::string& filename) override {
        string utopia_license =
            "//===----------------------------------------------------------------------===//\n"
            "//\n"
            "// Part of the Utopia EDA Project, under the Apache License v2.0\n"
            "// SPDX-License-Identifier: Apache-2.0\n"
            "// Copyright 2023 ISP RAS (http://www.ispras.ru)\n"
            "//\n"
            "//===----------------------------------------------------------------------===//\n";

        string text = component_->getText(filename);
        text =utopia_license+"\n"+text;
        return text;
    }
};

int main() {
    TextComponent* text = new PlainText();
    cout << "Enter namefile:\n";
    string filename;
    cin >> filename;
    
    cout << "\nCurrent text:\n" << text->getText(filename) << "\n";

    for (int i = 0; i < 3; ++i) {
        std::cout << "\nEnter mode: 1 - logo, 2 - license, 3 - border\n";
        string tmp;
        cin >> tmp;
        if (tmp == "1") {
            text = new LogoDecorator(text);
            cout << "\n" << text->getText(filename) << "\n";
        }
        else if (tmp == "2") {
            text = new CompanyLicenseDecorator(text);
            cout << "\n" << text->getText(filename) << "\n";
        }
        else if (tmp == "3") {
            text = new BorderDecorator(text);
            cout << "\n" << text->getText(filename) << "\n";
        }
        else {
            cout << "Repeat input\n";
            --i;
        }
    }
    delete text;
}
