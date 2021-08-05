#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Check whether target has an indirect path to source; return true if has indirect path, false if no path
bool check_path(int source, int target);

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    bool matching = false;
    // Loop through all the candidates and look for one called name;
    for (int i = 0; i < candidate_count; i++)
    {
        // TODO: Disallow people from voting the same person more than once.
        if (strcmp(name, candidates[i]) == 0)
        {
            matching = true;
            ranks[rank] = i; // candidates[i] is voted as the person's rank-th preference.
            return true;
        }
    }

    // if not found, dont update ranks; return false
    printf("Name not found\n");
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // Loop through all the candidate preferences
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            // Compare preferences and add winner and loser to a pair
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // Use any sorting algorithm but be as efficient as possible
    // sov = preferences[winner][loser]
    for (int i = 1; i < candidate_count; i++)
    {
        for (int j = i; j > 0; j--)
        {
            // Check if the preferences are out of order
            if (preferences[pairs[j].winner][pairs[j].loser] > preferences[pairs[j - 1].winner][pairs[j - 1].loser])
            {
                int w = pairs[j].winner;
                int l = pairs[j].loser;
                pairs[j].winner = pairs[j - 1].winner;
                pairs[j].loser = pairs[j - 1].loser;
                pairs[j - 1].winner = w;
                pairs[j - 1].loser = l;
            }
        }
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // Starting from first in sorted pairs array, set to true
    // Winner -> Loser ;
    // Dont add edge if create cycle
    // If an edge will connect a person who is not edged and everyone else is edged, then skip
    for (int i = 0; i < pair_count; i++)
    {
        bool willCycle = check_path(pairs[i].winner, pairs[i].loser);
        if (!willCycle)
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    // Go through all candidates in the locked array and whoever is
    for (int i = 0; i < candidate_count; i++)
    {
        bool gotLocked = false;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i]) // Check if the candidate has been locked on by someone
            {
                gotLocked = true;
            }
        }
        if (!gotLocked) // A candidate is the winner if no one locks them (they are the source)
        {
            printf("%s\n", candidates[i]);
        }
    }
    return;
}

// Check whether target has an indirect path to source; return true if has indirect path, false if no path
bool check_path(int source, int target)
{
    // If has edge, check all edges
    // If all edges return false, then return false
    // If has edge to source, then return true
    if (locked[target][source] == true)
    {
        return true;
    }
    // If no edge to source, return false
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[target][i] == true)
        {
            if (check_path(source, i) == true)
            {
                return true;
            }
        }
    }
    return false;
}