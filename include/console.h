#pragma once

#include <string>
#include <map>
#include <type_traits>
#include "app.h"

/// Уровень вывода
/**
 * При выводе отладочной информации сообщениям присваивается уровень важности.
 * Сообщения с низким уровнем важности будут проигнорированы. За минимальный допустимый
 * уровень важности отвечает переменная ```verbosity``` (см. \ref Console::getVariable())
 * */
enum Verbosity {
    VERB_DEBUG,
    VERB_INFO,
    VERB_ERROR
};

/// Командный интерпретатор
/**
 * Главный класс командного интерпретатора, содержащий указатели на приложения (см. \ref IApp),
 * переменные, потоки ввода-вывода.
 * */
class Console {
    public:
        Console(std::istream& in, std::ostream& out) : out_(out), in_(in) {}
        ~Console();

        /**
         * Создаёт приложение класса ```App```
         * \param [in] appName команда, связанная с приложением
         * \param [in] ...args аргументы конструктора
         * \return ```true```, если приложение успешно добавлено, и ```false```,
         * если
         * */
        template<class App, class... Args>
        bool emplaceApp(const std::string& appName, Args&& ...args) {
            if (apps_.find(appName) != apps_.end()) {
                return false;
            }
            apps_.emplace(std::make_pair(appName, new App(this, std::forward<Args>(args)...)));
            return true;
        }

        /**
         * Возвращает поток вывода
         * */
        std::ostream& output() const;

        /**
         * Возвращает поток логирования
         * \param [in] verbosity уровень важности
         * \param [in] prompt заголовок
         * */
        std::ostream& log(Verbosity verbosity, const char* prompt = "") const;

        /** Эквивалентно ```log(VERB_DEBUG, "# [DEBUG] ")``` (см. \ref log) */
        std::ostream& debug() const;

        /** Эквивалентно ```log(VERB_INFO, "# [INFO ] ")``` (см. \ref log) */
        std::ostream& info() const;

        /** Эквивалентно ```log(VERB_ERROR, "# [ERROR] ")``` (см. \ref log) */
        std::ostream& error() const;

        /** Возвращает поток ввода */
        std::istream& input() const;

        /** Возвращает уровень важности, исходя из значения переменной ```verbosity``` (см. \ref getVariable) */
        Verbosity getVerbosity() const;

        /** Исполняет основной цикл командного интерпретатора */
        int exec(int argc, char* argv[]);

        /** Возвращает значение переменной
         * \param [in] name название переменной
         * \param [in] defaultValue если переменная с именем ```name``` не установлена, вызов вернёт ```defaultValue``` */
        std::string getVariable(const std::string& name, const std::string& defaultValue = "") const;

        /** Устанавливает значение переменной ```name``` равным ```value```
         * */
        void setVariable(const std::string& name, const std::string& value);

        /** Возвращает все установленные приложения в виде отображения "команда" -> "приложение"
         * */
        const std::map<std::string, IApp*>& getApps() const;

        /** Возвращает все установленные переменные в виде отображения "название" -> "значение"
         * */
        const std::map<std::string, std::string>& getAllVariables() const;

    private:
        std::ostream& out_;
        std::istream& in_;
        std::map<std::string, IApp*> apps_;
        std::map<std::string, std::string> variables_;
};
