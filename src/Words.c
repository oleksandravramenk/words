#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <Windows.h>

#define MAX_WORD_LENGTH 100 // Визначення максимальної довжини слова

// Функція оновлення бази даних, після додавання нового слова
void updateDB(const char *filename, const char *word)
{
    FILE *file = fopen(filename, "a"); // Відкриття файлу зі словами для додавання нового слова
    if (file == NULL)
    {
        printf("При відкритті %s виникла помилка\n", filename);
        return;
    }

    fprintf(file, "%s\n", word); // Додавання нового слова у файл
    fclose(file);
}

// Функція для перевірки слова в базі даних
int WordValid(const char *filename, const char *word)
{
    FILE *file = fopen(filename, "r"); // Відкриття файлу зі словами для читання
    if (file == NULL)
    {
        printf("При відкритті %s виникла помилка\n", filename);
        return 0;
    }

    char buffer[MAX_WORD_LENGTH];
    while (fgets(buffer, sizeof(buffer), file)) // Читання кожного рядка з файлу зі словами
    {
        buffer[strcspn(buffer, "\n")] = '\0'; // Видалення символа нового рядка
        if (strcmp(buffer, word) == 0)        // Порівняння слова з рядка файлу із введеним словом для перевірки, чи існує введене слово в файлі
        {
            fclose(file);
            return 1; // Закриття файлу після знаходження слова в файлі
        }
    }

    fclose(file);
    return 0;
}

// Функція для перевірки, чи було слово вже використане
int WordUsed(char usedWords[][MAX_WORD_LENGTH], int usedCount, const char *word)
{
	int i;
    for (i = 0; i < usedCount; i++)
    {
        if (strcmp(usedWords[i], word) == 0)
        {
            return 1; // Слово вже було використане
        }
    }
    return 0; // Слово не було використане
}

// Функція отримання випадкового слова, яке починаться на задану літеру
char *GetRandomWord(const char *filename, char startLetter, char usedWords[][MAX_WORD_LENGTH], int usedCount)
{
	int i;
	
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("При відкритті %s виникла помилка\n", filename);
        return NULL;
    }

    char buffer[MAX_WORD_LENGTH];
    char *words[1000]; // Масив, в якому зберігаються слова, що починаються з заданої літери
    int count = 0;     // Лічильник кількості знайдених слів

    while (fgets(buffer, sizeof(buffer), file))
    {
        buffer[strcspn(buffer, "\n")] = '\0';
        if (buffer[0] == startLetter && !WordUsed(usedWords, usedCount, buffer)) // Перевірка першої літери слова і чи слово вже використане
        {
            words[count] = malloc(strlen(buffer) + 1); // Виділення пам'яті під слово
            strcpy(words[count], buffer); // Копіювання слова в масив
            count++; // Збільшення лічильника
        }
    }

    fclose(file);

    if (count == 0)
    {
        return NULL;
    }

    srand(time(0)); // Ініціалізація генератора
    int randomIndex = rand() % count; // Генерування випадкового індекса
    char *randomWord = malloc(strlen(words[randomIndex]) + 1); // Виділення пам'яті під слово
    strcpy(randomWord, words[randomIndex]); // Копіювання випадкового слова

    for (i = 0; i < count; i++)
    {
        free(words[i]); // Звільнення пам'яті для кожного слова
    }

    return randomWord; // Повернення випадкового слова
}

void bot()
{
    int i, topicChoice;

    char topicFiles[5][20] = {"animals.txt", "plants.txt", "cities.txt", "fruits.txt", "jobs.txt"};
    char topicNames[5][25] = {"Тварини", "Рослини", "Міста", "Фрукти/овочі", "Професії"};

    char usedWords[1000][MAX_WORD_LENGTH]; // Масив для збереження використаних слів
    int usedCount; // Лічильник використаних слів
    usedCount = 0;

    printf("\nОберіть тему:\n");
    for (i = 0; i < 5; i++)
    {
        printf("%d. %s\n", i + 1, topicNames[i]); // Виведення списка тем
    }

    printf("--> ");
    scanf("%d", &topicChoice);
    topicChoice--;

    const char *currentTopic = topicFiles[topicChoice]; // Вибрання певного файла теми слів
    printf("Тема: %s\n\n", topicNames[topicChoice]);

    char word[MAX_WORD_LENGTH];
    printf("Слово: ");
    scanf("%s", word);

    if (strcmp(word, "вихід") == 0)
    {
        exit(0); // Вихід
    }
    if (strcmp(word, "здаюся") == 0)
    {
        printf("\nВи здалися. Гра завершена.\n\n");
        getch();
        main(); // Завершення гри та повернення до меню
    }

    while (1)
    {
        if (!WordValid(currentTopic, word)) // Перевірка слова на його наявність
        {
            printf("Це слово неправильне, або його немає в базі даних.\nВи впевнені, що ввели правильно? (Так/Ні): ");

            char answer[4];
            scanf("%s", answer);

            if (strcmp(answer, "Так") == 0 || strcmp(answer, "так") == 0)
            {
                updateDB(currentTopic, word); // Додавання слова в базу даних
            }
            if (strcmp(answer, "вихід") == 0)
            {
                exit(0);
            }
            if (strcmp(answer, "здаюся") == 0)
            {
                printf("\nВи здалися. Гра завершена.\n\n");
                getch();
                main();
            }
        }

        if (WordUsed(usedWords, usedCount, word)) // Перевірка, чи слово вже було використане
        {
            printf("Це слово вже було використане. Введіть інше слово: ");
            scanf("%s", word);
            if (strcmp(word, "вихід") == 0)
            {
                exit(0);
            }
            if (strcmp(word, "здаюся") == 0)
            {
                printf("\nВи здалися. Гра завершена.\n\n");
                getch();
                main();
            }
            continue;
        }

        strcpy(usedWords[usedCount], word); // Додавання слова до використаних
        usedCount++;

        char lastChar = word[strlen(word) - 1]; // Зчитування останньої літери слова
        if (lastChar == 'ь' || lastChar == 'и')
        {
            lastChar = word[strlen(word) - 2]; // Якщо в слові останньою буквою буде 'и' або 'ь', то зчитає передостанній символ
        }

        char *computerWord = GetRandomWord(currentTopic, lastChar, usedWords, usedCount); // Отримання випадкового слова від комп'ютера
        if (computerWord == NULL)
        {
            printf("Комп'ютер не знайшов слово на літеру '%c'. Ви перемогли!\n\n", lastChar);
            break;
        }

        printf("Комп'ютер: %s\n", computerWord);

        strcpy(usedWords[usedCount], computerWord);
        usedCount++;

        lastChar = computerWord[strlen(computerWord) - 1];
        if (lastChar == 'ь' || lastChar == 'и')
        {
            lastChar = computerWord[strlen(computerWord) - 2];
        }

        printf("Введіть слово, що починається з літери '%c': ", lastChar);
        scanf("%s", word);

        if (strcmp(word, "вихід") == 0)
        {
            exit(0);
        }
        if (strcmp(word, "здаюся") == 0)
        {
            printf("\nВи здалися. Гра завершена.\n\n");
            getch();
            main();
        }
        while (word[0] != lastChar) // Якщо слово буде починатися не з тієї букви, видасть попередження
        {
            printf("Ви повинні назвати слово, що починається з літери '%c'.\n", lastChar);
            printf("Спробуйте ще раз: ");
            scanf("%s", word);
            if (strcmp(word, "вихід") == 0)
            {
                exit(0);
            }
            if (strcmp(word, "здаюся") == 0)
            {
                printf("\nВи здалися. Гра завершена.\n\n");
                getch();
                main();
            }
        }

        strcpy(word, word);
    }
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int i, a, topicChoice;

    char topicFiles[5][20] = {"animals.txt", "plants.txt", "cities.txt", "fruits.txt", "jobs.txt"};
    char topicNames[5][25] = {"Тварини", "Рослини", "Міста", "Фрукти/овочі", "Професії"};

    while (1)
    {
        system("cls");
        Sleep(500);
        printf("************МЕНЮ****************\n\n\n");
        Sleep(500);
        printf("1. Почати гру.\n");
        Sleep(500);
        printf("2. Інструкція.\n");
        Sleep(500);
        printf("3. Вихід.\n");
        Sleep(500);
        printf("\n\n----------------------------");
        Sleep(500);
        printf("\n> ");
        scanf("%d", &a);
        system("cls");

        if (a == 1)
        {
            while (1)
            {
                printf("Вибір режиму гри:\n");
                printf("1. З комп'ютером.\n");
                printf("2. З іншим гравцем.\n");
                printf("3. Повернутися до головного меню.\n");
                printf("> ");
                scanf("%d", &a);
                if (a == 1)
                {
                    bot();
                }
                if (a == 2)
                {
                    printf("\nОберіть тему:\n");
                    for (i = 0; i < 5; i++)
                    {
                        printf("%d. %s\n", i + 1, topicNames[i]);
                    }

                    printf("--> ");
                    scanf("%d", &topicChoice);
                    topicChoice--;

                    const char *currentTopic = topicFiles[topicChoice];
                    printf("Тема: %s\n\n", topicNames[topicChoice]);

                    char word[MAX_WORD_LENGTH];
                    char previousWord[MAX_WORD_LENGTH] = "";
                    printf("Слово: ");
                    scanf("%s", word);

                    if (strcmp(word, "вихід") == 0)
                    {
                        exit(0);
                    }
                    if (strcmp(word, "здаюся") == 0)
                    {
                        printf("\nВи здалися. Гра завершена.\n\n");
                        break;
                    }

                    while (1)
                    {
                        if (!WordValid(currentTopic, word))
                        {
                            printf("Це слово неправильне, або його немає в базі даних.\nВи впевнені, що ввели правильно? (Так/Ні): ");

                            char answer[4];
                            scanf("%s", answer);

                            if (strcmp(answer, "Так") == 0 || strcmp(answer, "так") == 0)
                            {
                                updateDB(currentTopic, word);
                            }
                            else
                            {
                                strcpy(word, previousWord);
                            }
                            if (strcmp(answer, "вихід") == 0)
                            {
                                exit(0);
                            }
                            if (strcmp(answer, "здаюся") == 0)
                            {
                                printf("\nВи здалися. Гра завершена.\n\n");
                                break;
                            }
                        }

                        char lastChar = word[strlen(word) - 1];
                        if (lastChar == 'ь' || lastChar == 'и')
                        {
                            lastChar = word[strlen(word) - 2];
                        }

                        printf("Введіть слово, що починається з літери '%c': ", lastChar);

                        char nextword[MAX_WORD_LENGTH];
                        scanf("%s", nextword);
                        if (strcmp(nextword, "здаюся") == 0)
                        {
                            printf("\nВи здалися. Гра завершена.\n\n");
                            break;
                        }
                        if (strcmp(nextword, "вихід") == 0)
                        {
                            exit(0);
                        }

                        while (nextword[0] != lastChar)
                        {
                            printf("Ви повинні назвати слово, що починається з літери '%c'.\n", lastChar);
                            printf("Спробуйте ще раз: ");
                            scanf("%s", nextword);
                        }

                        strcpy(previousWord, word);
                        strcpy(word, nextword);
                    }
                }
                if (a == 3)
                {
                    break; // Повернення до головного меню
                }
            }
        }
        else if (a == 2)
        {
            printf("Правила\n\n");
            printf("Гра в слова – гра, в якій один гравець називає слово, а інший називає інше на останню букву слова першого гравця.\n");
            printf("Щоб почати гру, спочатку потрібно вибрати режим: з комп'ютером чи з іншим гравцем.\n");
            printf("В залежності від вибору режиму, буде запропоновано на вибір 5 тем: міста, тварини, рослини, фрукти та овочі, професії.\n");
            printf("Якщо вибрано режим гри з комп'ютером, то комп'ютер назве випадкове слово, а гравець повинен написати інше на останню\n");
            printf("букву слова, яке назвав комп'ютер, потім комп'ютер назве інше слово на останню букву слова гравця і т.д.\n");
            printf("Якщо вибрано режим гри з іншим гравцем, то один гравець повинен назвати якесь слово, інший називає нове слово\n");
            printf("на останню букву слова першого гравця і т.д.\n");
            printf("Щоб повернутися до головного меню, напишіть слово 'здаюся', а щоб вийти з гри, напишіть слово 'вихід'.\n\n");
            printf("Приємної гри :)\n");
            printf("\n\n\nДля продовження натисніть Enter...");
            getch();
        }
        else if (a == 3)
        {
            printf("Дякую, що зіграли :)");
            getch();
            exit(0);
        }
        else
        {
            printf("Неправильний вибір. Спробуйте ще раз.\n");
        }
    }

    return 0;
}
