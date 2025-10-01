#include <stdio.h>
#include <stdlib.h>

#define BALANCE_FILE "balance.txt"

void clear_input() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

double load_balance() {
    FILE *f = fopen(BALANCE_FILE, "r");
    if (!f) {
        return 0.0; // default balance if file doesn't exist
    }
    double bal = 0.0;
    if (fscanf(f, "%lf", &bal) != 1) {
        bal = 0.0;
    }
    fclose(f);
    return bal;
}

int save_balance(double bal) {
    FILE *f = fopen(BALANCE_FILE, "w");
    if (!f) {
        perror("Error opening balance file for writing");
        return 0;
    }
    fprintf(f, "%.2f\n", bal);
    fclose(f);
    return 1;
}

void print_menu() {
    printf("\n=== ATM Simulator ===\n");
    printf("1) Deposit\n");
    printf("2) Withdraw\n");
    printf("3) Check Balance\n");
    printf("4) Exit\n");
    printf("Choose an option (1-4): ");
}

int main(void) {
    double balance = load_balance();
    int choice = 0;
    int running = 1;

    while (running) {
        print_menu();
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number 1-4.\n");
            clear_input();
            continue;
        }

        if (choice == 1) {
            double amt;
            printf("Enter amount to deposit: ");
            if (scanf("%lf", &amt) != 1 || amt <= 0) {
                printf("Invalid amount.\n");
                clear_input();
                continue;
            }
            balance += amt;
            if (!save_balance(balance)) {
                printf("Warning: could not save balance!\n");
            }
            printf("Deposited %.2f. New balance: %.2f\n", amt, balance);
        } else if (choice == 2) {
            double amt;
            printf("Enter amount to withdraw: ");
            if (scanf("%lf", &amt) != 1 || amt <= 0) {
                printf("Invalid amount.\n");
                clear_input();
                continue;
            }
            if (amt > balance) {
                printf("Insufficient funds. Current balance: %.2f\n", balance);
                continue;
            }
            balance -= amt;
            if (!save_balance(balance)) {
                printf("Warning: could not save balance!\n");
            }
            printf("Withdrew %.2f. New balance: %.2f\n", amt, balance);
        } else if (choice == 3) {
            printf("Current balance: %.2f\n", balance);
        } else if (choice == 4) {
            printf("Goodbye!\n");
            running = 0;
        } else {
            printf("Please choose between 1 and 4.\n");
        }
    }

    // Save on exit too
    save_balance(balance);
    return 0;
}