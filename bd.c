#include <stdio.h>
#include <string.h>

// Определение структуры, соответствующей таблице "dish"
struct Dish
{
    int dish_id;         // dish_id INT (Первичный ключ)
    char dish_name[150]; // dish_name VARCHAR(150)
    double price;        // price DECIMAL(10,2)
};

int main()
{
    // Создание таблицы в виде массива структур
    // В данном примере добавляем 3 записи
    struct Dish dish_table[3];

    // Заполнение первой записи (строки таблицы)
    dish_table[0].dish_id = 1;
    strcpy(dish_table[0].dish_name, "Пицца Маргарита");
    dish_table[0].price = 550.00;

    // Заполнение второй записи
    dish_table[1].dish_id = 2;
    strcpy(dish_table[1].dish_name, "Паста Карбонара");
    dish_table[1].price = 480.50;

    // Заполнение третьей записи
    dish_table[2].dish_id = 3;
    strcpy(dish_table[2].dish_name, "Салат Цезарь");
    dish_table[2].price = 390.00;

    // Вывод содержимого «таблицы» на экран
    printf("Таблица: dish\n");
    printf("---------------------------------------------------------\n");
    printf("| %-7s | %-25s | %-15s |\n", "dish_id", "dish_name", "price");
    printf("---------------------------------------------------------\n");

    for (int i = 0; i < 3; i++)
    {
        printf("| %-7d | %-25s | %-15.2f |\n",
               dish_table[i].dish_id,
               dish_table[i].dish_name,
               dish_table[i].price);
    }

    printf("---------------------------------------------------------\n");

    return 0;
}
