#include "MaxProfit.h"
using namespace std;

int sum(int * prices, int size) {
    int retVal = 0;
    for (int i = 0; i < size; i++) {
        retVal += prices[i];
    }
    return retVal;
}

int lookup_price(vector<char *> cards_set, char ** names, int * prices, int cards_in_problem) { // return price of each subset of cards
    // get prices of cards in cards_set by checking with names
    int total = 0;
    // int index = 0;
    for (int i = 0; i < cards_set.size(); i++) { // loop through cards in cards_set
        for (int j = 0; j < cards_in_problem; j++) { // search for matching card in names
            if (strcmp(cards_set[i], names[j]) == 0) {
                total += prices[j];
            }
        }
    }
    // printf("total price = %d", total);
    return total;
}

int compute_profit(vector<char *> cards_set, int market_num, char ** market_names, int * market_values) {
    int profit = 0;
    for (int i = 0; i < cards_set.size(); i++) { // loop through cards in cards_set
        int t_or_f = 0;
        for (int j = 0; j < market_num; j++) { // search for matching card in market_names
            if (strcmp(cards_set[i], market_names[j]) == 0) {
                t_or_f = 1;
                profit += market_values[j];
            }
        }
        if (t_or_f == 0) {
            printf("Error: price list contains a card that is not available in the market prices index.\n");
            exit(EXIT_FAILURE);
        }
    }
    return profit;
}

void generate_subsequences(char ** names, int * prices, int num_sets, int cards_in_problem, int funds, int market_num, char ** market_names, int * market_values, FILE * output_file) {
    int counter = 1;
    int max_profit = 0; // stores max_profit
    int num_card_in_max = 0; // stores # of cards in max set
    int price_of_subset = -1;
    vector<char *> max_subset; // will store subset with max profit

    auto time1 = chrono::high_resolution_clock::now();
    while (counter <= num_sets) {
        vector<char *> subset;
        int count_mod = counter;

        int i = 0;
        while (count_mod > 0) {

            if (count_mod % 2 == 1) {
                subset.push_back(names[i]);
            }
            count_mod = count_mod / 2;
            i++;
        }

        price_of_subset = lookup_price(subset, names, prices, cards_in_problem);
        // printf("subset price: %d\n", price_of_subset);

        if (price_of_subset <= funds) { // subset is valid
            // get market value of cards
            // printf("price of subset: %d money to spend: %d\n", price_of_subset, funds);
            if (compute_profit(subset, market_num, market_names, market_values) - price_of_subset > max_profit) {
                max_profit = compute_profit(subset, market_num, market_names, market_values) - price_of_subset;
                num_card_in_max = subset.size();
            }
        }
        counter++;
    }
    auto time2 = chrono::high_resolution_clock::now();
    auto elapsed = (float) (chrono::duration_cast<chrono::microseconds>(time2 - time1).count()) / 1000000;
    // printf("cards in problem %d, max profit %d, num cards purchased %d, time elapsed %lf seconds\n", cards_in_problem, max_profit, num_card_in_max, elapsed);
    fprintf(output_file, "%d %d %d %lf\n", cards_in_problem, max_profit, num_card_in_max, elapsed);
    // FINAL OUTPUT
    // fprintf(cards_in_problem, max_profit, max_subset.size(), time??)

}

void execute_problems(int total_cards, char ** card_names, int * card_values, char * filename, int market_num) {
    FILE * prices_file;
    int num_cards_in_problem; // # of cards in each individual problem
    int money_to_spend; // amount of money allotted in each individual problem

    prices_file = fopen(filename, "r");
    FILE * output_file;
    output_file = fopen("output.txt", "w");
    while (fscanf(prices_file, "%d %d", &num_cards_in_problem, &money_to_spend) == 2) {
        // printf("cards in problem: %d\n", num_cards_in_problem);
        // printf("money to spend: %d\n", money_to_spend);

        // inner loop to get cards in each problem
        char * cards[num_cards_in_problem];
        int card_prices[num_cards_in_problem];
        for (int i = 0; i < num_cards_in_problem; i++) {
            cards[i] = (char *) malloc (64*sizeof(char));
            fscanf(prices_file, "%s %d", cards[i], &card_prices[i]);
        }
        /*
        for (int i = 0; i < num_cards_in_problem; i++) {
            printf("name %s price %d\n", cards[i], card_prices[i]);
        }
        */
        int num_sets = pow(2, num_cards_in_problem) - 1;
        // printf("Subsets in set: %d\n", num_sets);

        generate_subsequences(cards, card_prices, num_sets, num_cards_in_problem, money_to_spend, market_num, card_names, card_values, output_file);
    }

    fclose(output_file);


}



int main(int argc, char *argv[]) {
    // ./MaxProfit is argv[0]
    // program1 –m <market-price-file> -p <price-list-file>
    /*
    printf("Argc = %d\n", argc);
    for (int i = 0; i < argc; i++) {
        printf("Argv[%d] = %s\n", i, argv[i]);
    }
    */
    FILE * market_file;
    int market_num;

    market_file = fopen(argv[2], "r");
    fscanf(market_file, "%d", &market_num);
    // printf("num cards: %d\n", market_num);

    char * market_names[market_num];
    int market_values[market_num];

    for (int i = 0; i < market_num; i++) {

        market_names[i] = (char *) malloc (64*sizeof(char));
        fscanf(market_file, "%s %d", market_names[i], &market_values[i]);

        //market_names.push_back(curr_name);

        // printf("current name %s\n", market_names[i]);
        // printf("current price %d\n", market_values[i]);
    }
    fclose(market_file);
    /*
    for (int i = 0; i < market_num; i++) {
        printf("name: %s value: %d\n", market_names[i], market_values[i]);
    }
    printf("---------------------------\n");
    */

    char * pricefile = argv[4]; // name of file used for price_list
    execute_problems(market_num, market_names, market_values, pricefile, market_num);


    // NOTES
    // while loop scanning lines from price_list
    // inner for loop for each card in a problem
    // fcanf %d %s to get string and price
    // while (fscanf(market_file, "%s %d", curr_name, &curr_price) == 2) (LIKE THIS)
    // MUST compare character by character when matching card names
    // POWER SET for obtaining each subset

    // compare strings: if (strcmp(1,2) == 0)


}
