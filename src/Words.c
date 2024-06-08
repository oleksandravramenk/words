#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <Windows.h>

#define MAX_WORD_LENGTH 100 // ���������� ����������� ������� �����

// ������� ��������� ���� �����, ���� ��������� ������ �����
void updateDB(const char *filename, const char *word)
{
    FILE *file = fopen(filename, "a"); // ³������� ����� � ������� ��� ��������� ������ �����
    if (file == NULL)
    {
        printf("��� ������� %s ������� �������\n", filename);
        return;
    }

    fprintf(file, "%s\n", word); // ��������� ������ ����� � ����
    fclose(file);
}

// ������� ��� �������� ����� � ��� �����
int WordValid(const char *filename, const char *word)
{
    FILE *file = fopen(filename, "r"); // ³������� ����� � ������� ��� �������
    if (file == NULL)
    {
        printf("��� ������� %s ������� �������\n", filename);
        return 0;
    }

    char buffer[MAX_WORD_LENGTH];
    while (fgets(buffer, sizeof(buffer), file)) // ������� ������� ����� � ����� � �������
    {
        buffer[strcspn(buffer, "\n")] = '\0'; // ��������� ������� ������ �����
        if (strcmp(buffer, word) == 0)        // ��������� ����� � ����� ����� �� �������� ������ ��� ��������, �� ���� ������� ����� � ����
        {
            fclose(file);
            return 1; // �������� ����� ���� ����������� ����� � ����
        }
    }

    fclose(file);
    return 0;
}

// ������� ��� ��������, �� ���� ����� ��� �����������
int WordUsed(char usedWords[][MAX_WORD_LENGTH], int usedCount, const char *word)
{
	int i;
    for (i = 0; i < usedCount; i++)
    {
        if (strcmp(usedWords[i], word) == 0)
        {
            return 1; // ����� ��� ���� �����������
        }
    }
    return 0; // ����� �� ���� �����������
}

// ������� ��������� ����������� �����, ��� ���������� �� ������ �����
char *GetRandomWord(const char *filename, char startLetter, char usedWords[][MAX_WORD_LENGTH], int usedCount)
{
	int i;
	
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("��� ������� %s ������� �������\n", filename);
        return NULL;
    }

    char buffer[MAX_WORD_LENGTH];
    char *words[1000]; // �����, � ����� ����������� �����, �� ����������� � ������ �����
    int count = 0;     // ˳������� ������� ��������� ���

    while (fgets(buffer, sizeof(buffer), file))
    {
        buffer[strcspn(buffer, "\n")] = '\0';
        if (buffer[0] == startLetter && !WordUsed(usedWords, usedCount, buffer)) // �������� ����� ����� ����� � �� ����� ��� �����������
        {
            words[count] = malloc(strlen(buffer) + 1); // �������� ���'�� �� �����
            strcpy(words[count], buffer); // ��������� ����� � �����
            count++; // ��������� ���������
        }
    }

    fclose(file);

    if (count == 0)
    {
        return NULL;
    }

    srand(time(0)); // ����������� ����������
    int randomIndex = rand() % count; // ����������� ����������� �������
    char *randomWord = malloc(strlen(words[randomIndex]) + 1); // �������� ���'�� �� �����
    strcpy(randomWord, words[randomIndex]); // ��������� ����������� �����

    for (i = 0; i < count; i++)
    {
        free(words[i]); // ��������� ���'�� ��� ������� �����
    }

    return randomWord; // ���������� ����������� �����
}

void bot()
{
    int i, topicChoice;

    char topicFiles[5][20] = {"animals.txt", "plants.txt", "cities.txt", "fruits.txt", "jobs.txt"};
    char topicNames[5][25] = {"�������", "�������", "̳���", "������/�����", "������"};

    char usedWords[1000][MAX_WORD_LENGTH]; // ����� ��� ���������� ������������ ���
    int usedCount; // ˳������� ������������ ���
    usedCount = 0;

    printf("\n������ ����:\n");
    for (i = 0; i < 5; i++)
    {
        printf("%d. %s\n", i + 1, topicNames[i]); // ��������� ������ ���
    }

    printf("--> ");
    scanf("%d", &topicChoice);
    topicChoice--;

    const char *currentTopic = topicFiles[topicChoice]; // �������� ������� ����� ���� ���
    printf("����: %s\n\n", topicNames[topicChoice]);

    char word[MAX_WORD_LENGTH];
    printf("�����: ");
    scanf("%s", word);

    if (strcmp(word, "�����") == 0)
    {
        exit(0); // �����
    }
    if (strcmp(word, "������") == 0)
    {
        printf("\n�� �������. ��� ���������.\n\n");
        getch();
        main(); // ���������� ��� �� ���������� �� ����
    }

    while (1)
    {
        if (!WordValid(currentTopic, word)) // �������� ����� �� ���� ��������
        {
            printf("�� ����� �����������, ��� ���� ���� � ��� �����.\n�� �������, �� ����� ���������? (���/ͳ): ");

            char answer[4];
            scanf("%s", answer);

            if (strcmp(answer, "���") == 0 || strcmp(answer, "���") == 0)
            {
                updateDB(currentTopic, word); // ��������� ����� � ���� �����
            }
            if (strcmp(answer, "�����") == 0)
            {
                exit(0);
            }
            if (strcmp(answer, "������") == 0)
            {
                printf("\n�� �������. ��� ���������.\n\n");
                getch();
                main();
            }
        }

        if (WordUsed(usedWords, usedCount, word)) // ��������, �� ����� ��� ���� �����������
        {
            printf("�� ����� ��� ���� �����������. ������ ���� �����: ");
            scanf("%s", word);
            if (strcmp(word, "�����") == 0)
            {
                exit(0);
            }
            if (strcmp(word, "������") == 0)
            {
                printf("\n�� �������. ��� ���������.\n\n");
                getch();
                main();
            }
            continue;
        }

        strcpy(usedWords[usedCount], word); // ��������� ����� �� ������������
        usedCount++;

        char lastChar = word[strlen(word) - 1]; // ���������� �������� ����� �����
        if (lastChar == '�' || lastChar == '�')
        {
            lastChar = word[strlen(word) - 2]; // ���� � ���� ��������� ������ ���� '�' ��� '�', �� ����� ������������ ������
        }

        char *computerWord = GetRandomWord(currentTopic, lastChar, usedWords, usedCount); // ��������� ����������� ����� �� ����'�����
        if (computerWord == NULL)
        {
            printf("����'���� �� ������� ����� �� ����� '%c'. �� ���������!\n\n", lastChar);
            break;
        }

        printf("����'����: %s\n", computerWord);

        strcpy(usedWords[usedCount], computerWord);
        usedCount++;

        lastChar = computerWord[strlen(computerWord) - 1];
        if (lastChar == '�' || lastChar == '�')
        {
            lastChar = computerWord[strlen(computerWord) - 2];
        }

        printf("������ �����, �� ���������� � ����� '%c': ", lastChar);
        scanf("%s", word);

        if (strcmp(word, "�����") == 0)
        {
            exit(0);
        }
        if (strcmp(word, "������") == 0)
        {
            printf("\n�� �������. ��� ���������.\n\n");
            getch();
            main();
        }
        while (word[0] != lastChar) // ���� ����� ���� ���������� �� � 򳺿 �����, ������� ������������
        {
            printf("�� ������ ������� �����, �� ���������� � ����� '%c'.\n", lastChar);
            printf("��������� �� ���: ");
            scanf("%s", word);
            if (strcmp(word, "�����") == 0)
            {
                exit(0);
            }
            if (strcmp(word, "������") == 0)
            {
                printf("\n�� �������. ��� ���������.\n\n");
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
    char topicNames[5][25] = {"�������", "�������", "̳���", "������/�����", "������"};

    while (1)
    {
        system("cls");
        Sleep(500);
        printf("************����****************\n\n\n");
        Sleep(500);
        printf("1. ������ ���.\n");
        Sleep(500);
        printf("2. ����������.\n");
        Sleep(500);
        printf("3. �����.\n");
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
                printf("���� ������ ���:\n");
                printf("1. � ����'������.\n");
                printf("2. � ����� �������.\n");
                printf("3. ����������� �� ��������� ����.\n");
                printf("> ");
                scanf("%d", &a);
                if (a == 1)
                {
                    bot();
                }
                if (a == 2)
                {
                    printf("\n������ ����:\n");
                    for (i = 0; i < 5; i++)
                    {
                        printf("%d. %s\n", i + 1, topicNames[i]);
                    }

                    printf("--> ");
                    scanf("%d", &topicChoice);
                    topicChoice--;

                    const char *currentTopic = topicFiles[topicChoice];
                    printf("����: %s\n\n", topicNames[topicChoice]);

                    char word[MAX_WORD_LENGTH];
                    char previousWord[MAX_WORD_LENGTH] = "";
                    printf("�����: ");
                    scanf("%s", word);

                    if (strcmp(word, "�����") == 0)
                    {
                        exit(0);
                    }
                    if (strcmp(word, "������") == 0)
                    {
                        printf("\n�� �������. ��� ���������.\n\n");
                        break;
                    }

                    while (1)
                    {
                        if (!WordValid(currentTopic, word))
                        {
                            printf("�� ����� �����������, ��� ���� ���� � ��� �����.\n�� �������, �� ����� ���������? (���/ͳ): ");

                            char answer[4];
                            scanf("%s", answer);

                            if (strcmp(answer, "���") == 0 || strcmp(answer, "���") == 0)
                            {
                                updateDB(currentTopic, word);
                            }
                            else
                            {
                                strcpy(word, previousWord);
                            }
                            if (strcmp(answer, "�����") == 0)
                            {
                                exit(0);
                            }
                            if (strcmp(answer, "������") == 0)
                            {
                                printf("\n�� �������. ��� ���������.\n\n");
                                break;
                            }
                        }

                        char lastChar = word[strlen(word) - 1];
                        if (lastChar == '�' || lastChar == '�')
                        {
                            lastChar = word[strlen(word) - 2];
                        }

                        printf("������ �����, �� ���������� � ����� '%c': ", lastChar);

                        char nextword[MAX_WORD_LENGTH];
                        scanf("%s", nextword);
                        if (strcmp(nextword, "������") == 0)
                        {
                            printf("\n�� �������. ��� ���������.\n\n");
                            break;
                        }
                        if (strcmp(nextword, "�����") == 0)
                        {
                            exit(0);
                        }

                        while (nextword[0] != lastChar)
                        {
                            printf("�� ������ ������� �����, �� ���������� � ����� '%c'.\n", lastChar);
                            printf("��������� �� ���: ");
                            scanf("%s", nextword);
                        }

                        strcpy(previousWord, word);
                        strcpy(word, nextword);
                    }
                }
                if (a == 3)
                {
                    break; // ���������� �� ��������� ����
                }
            }
        }
        else if (a == 2)
        {
            printf("�������\n\n");
            printf("��� � ����� � ���, � ��� ���� ������� ������ �����, � ����� ������ ���� �� ������� ����� ����� ������� ������.\n");
            printf("��� ������ ���, �������� ������� ������� �����: � ����'������ �� � ����� �������.\n");
            printf("� ��������� �� ������ ������, ���� ������������� �� ���� 5 ���: ����, �������, �������, ������ �� �����, ������.\n");
            printf("���� ������� ����� ��� � ����'������, �� ����'���� ����� ��������� �����, � ������� ������� �������� ���� �� �������\n");
            printf("����� �����, ��� ������ ����'����, ���� ����'���� ����� ���� ����� �� ������� ����� ����� ������ � �.�.\n");
            printf("���� ������� ����� ��� � ����� �������, �� ���� ������� ������� ������� ����� �����, ����� ������ ���� �����\n");
            printf("�� ������� ����� ����� ������� ������ � �.�.\n");
            printf("��� ����������� �� ��������� ����, �������� ����� '������', � ��� ����� � ���, �������� ����� '�����'.\n\n");
            printf("������ ��� :)\n");
            printf("\n\n\n��� ����������� �������� Enter...");
            getch();
        }
        else if (a == 3)
        {
            printf("�����, �� ������ :)");
            getch();
            exit(0);
        }
        else
        {
            printf("������������ ����. ��������� �� ���.\n");
        }
    }

    return 0;
}
