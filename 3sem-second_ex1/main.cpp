#include <iostream>
#include <string.h>
#include <vector>
#include <fstream>

/*
 * Размер алфавита, в котором работает разработчик, дефолтный размер - размер английского алфавита
 */
const size_t alph_size = 26;


/*
 * Находит наименьшую строчку(лексикографически), что её префикс функция равна заданной
 *
 */

class SmallestStringFinder {
public:
    /*
     * Explicit - чтобы избежать неявных преобразований
     *
     * Функция - конструктор, которая вычисляет наименьшую строчку, сохраняет ответ в приватное поле found_string
     * На вход принимает массив типа size_t, который равен префикс функии от строчки которую нужно найти.
     * Если нужно использовать класс и изменился алфавит, обратите внимение на комментарии в конструкторе.
     */
    explicit SmallestStringFinder(const std::vector<size_t>& prefix);

    /*
     * Возращает найденную строчку
     */
    std::string GetString() const;
private:
    /*
     *  Наименьшая лексикографическая строка, ответ на задачу
     */
    std::vector<char> found_string;
};

SmallestStringFinder::SmallestStringFinder(const std::vector<size_t>& prefix) {

    found_string.resize(prefix.size());
    found_string[0] = 'a'; // Минимальная буква в дефолтном алфавите

    for (size_t i = 1; i < prefix.size(); i++) {
        if (prefix[i] == 0) {
            std::vector<bool> used(alph_size, false);
            size_t prev = prefix[i - 1];
            while (prev > 0 ) {
                used[found_string[prev] - 'a'] = true; // Нужно вычесть минимальный символ алфавита
                prev = prefix[prev - 1];
            }
            for (size_t j = 1; j < alph_size; j++) {
                if (!used[j]) {
                    found_string[i] = (char) (j + 'a'); // Нужно прибавить минимальный символ алфавита
                    break;
                }
            }
        } else {
            found_string[i] = found_string[prefix[i] - 1];
        }
    }
}

std::string SmallestStringFinder::GetString() const {
    return std::string(found_string.begin(), found_string.end()); // превращаем вектор в стринг.
}

int main() {

    freopen("input.txt", "r", stdin);

    std::vector<size_t> prefix; // вектор, куда будем записывать значения префукс-функции

    size_t new_num;
    while (std::cin >> new_num) {
        prefix.push_back(new_num); // считывание значений префукс функции в вектор prefix
    }

    SmallestStringFinder smallestStringFinder = SmallestStringFinder(prefix); // инициализиование и вычисление строчки - рузельтата
    std::cout << smallestStringFinder.GetString(); // Выведение строчки на экран компьютера или другого устройства
}
