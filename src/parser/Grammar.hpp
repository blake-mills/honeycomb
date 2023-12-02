#pragma once
#include <vector>
#include <unordered_map>
#include <string>

struct Symbol {
    std::string value;

    bool cmp(const std::string& str) {
        if (value.size() >= 2 && value.front() == '"' && value.back() == '"') {
            std::string noQuotes = value.substr(1, str.size());
            return noQuotes == str;
        } else {
           return value == str;
        }
    }

    bool isNonTerminal() const {
        return (!value.empty() && value[0] == '<');
    }

    bool isLiteral() const {
        return !value.empty() && value[0] == '"' && value[value.size() - 1] == '"';
    }

    friend std::ostream& operator<<(std::ostream& os, const Symbol& s) {
        os << "Symbol(value = " << s.value << 
              ", isNonTerminal = " << std::boolalpha << s.isNonTerminal() << 
              ", isLiteral = " << std::boolalpha << s.isLiteral() <<
              ")";
        return os;
    }
};

struct Derivation {
    std::vector<Symbol> symbols;
    Symbol operator[](std::size_t index) {
        if (index < symbols.size()) {
            return symbols[index];
        }
        throw std::runtime_error("Index `" + std::to_string(index) + 
                                "` out of bounds [0, " + std::to_string(symbols.size() - 1) +
                                "] in Derivation");
    }
    friend std::ostream& operator<<(std::ostream& os, const Derivation& d) {
        os << "Derivation(symbols = [";
        for (std::size_t i = 0; i < d.symbols.size(); ++i) {
            os << d.symbols[i];
            if (i < d.symbols.size() - 1) {
                os << ", ";
            }
        }
        os << "])";
        return os;
    }
};

struct Derivations {
    std::vector<Derivation> derivations;
    Derivation operator[](std::size_t index) {
        if (index < derivations.size()) {
            return derivations[index];
        }
        throw std::runtime_error("Index `" + std::to_string(index) + 
                                "` out of bounds [0, " + std::to_string(derivations.size() - 1) +
                                "] in Derivation");
    }

    void addDerivation(const Derivation& d) {
        derivations.push_back(d);
    }

    void addDerivation(const std::vector<std::string>& l) {
        Derivation d;
        for (auto& s : l) {
            d.symbols.push_back({ s });
        }
        derivations.push_back(d);
    }

    friend std::ostream& operator<<(std::ostream& os, const Derivations& d) {
         os << "Derivations(derivations = [";
        for (std::size_t i = 0; i < d.derivations.size(); ++i) {
            os << d.derivations[i];
            if (i < d.derivations.size() - 1) {
                os << ", ";
            }
        }
        os << "])";
        return os;
    }

};

struct Grammar {
    std::unordered_map<std::string, Derivations> grammar;
    Derivations operator[](const std::string& i) {
        if (grammar.find(i) != grammar.end()) {
            return grammar[i];
        }
        throw std::runtime_error("No derivation found in grammar: " + i);
    }

    void addDerivation(const std::string& id, const std::vector<std::string>& l) {
        if (grammar.find(id) == grammar.end()) {
            Derivations ds;
            grammar[id] = ds;
        }
        grammar[id].addDerivation(l);
    }

    friend std::ostream& operator<<(std::ostream& os, const Grammar& g) {
        os << "{";
        std::size_t indentationCounter = 0;
        for (auto it = g.grammar.begin(); it != g.grammar.end(); ++it) {
            auto key = it->first;
            auto value = it->second;
            os << "\n\t" << key << ": Derivations(derivations = [\n";
            auto derivations = value.derivations;

            for (std::size_t i = 0; i < derivations.size(); ++i) {
                indentationCounter = 2;
                for (std::size_t j = 0; j < indentationCounter; ++j) {
                    os << "\t";
                }
                os << "Derivation(symbols = [\n";
                indentationCounter = 3;
                auto derivation = derivations[i];
                auto symbols = derivations[i].symbols;

                for (std::size_t j = 0; j < symbols.size(); ++j) {
                    for (std::size_t k = 0; k < indentationCounter; ++k) {
                        os << "\t";
                    }
                    if (j != symbols.size() - 1) {
                        os << symbols[j] << ",\n";
                    }
                    else {
                        os << symbols[j] << "\n";
                    }
                }
                indentationCounter = 2;
                for (std::size_t k = 0; k < indentationCounter; ++k) {
                    os << "\t";
                }
                os << "])\n";
            }
            os << "\t])";

            if (std::next(it) != g.grammar.end()) {
                os << ",\n";
            }
            else {
                os << "\n";
            }
        }
        os << "}";
        return os;
    }
};