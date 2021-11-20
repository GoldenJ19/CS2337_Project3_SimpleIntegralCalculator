/**
    Justin Hardy
    JEH180008
*/

#include "BinTree.h"
#include "Payload.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

string PrintEquation( BinaryTree<Payload>& tree ) {
    stringstream stream;
    // Get Tree's Reverse Order Output, and input into the stream.
    stream << tree.GetReverseInOrder();
    // Extract the string from the stringstream.
    string toReturn = stream.str();

    // Remove any double spaces
    while((int)toReturn.find("  ") != -1) {
        toReturn = toReturn.substr(0, toReturn.find("  ")) + toReturn.substr(toReturn.find("  ") + 1);
    }

    // Figure out which payload is the start of the string...
    // Doing this b/c there could be payloads that have a coefficient of 0.
    double startingCoefficient = 0;
    int i = 0;
    while( i < tree.Size() && startingCoefficient == 0 ) {
        // This will get the first coefficient that is not 0.
        // GetAt() uses indexing based off of reverse-inorder traversal.
        startingCoefficient = tree.GetAt(i).GetCoefficientAsDouble();
        i++;
    }

    // Reformat negative sign for the starting payload in the string.
    if(startingCoefficient != 0) {
        if( startingCoefficient < 0 ) {
            // Cut out the beginning minus sign from the starting negative number.
            toReturn = toReturn.substr( toReturn.find("-") + 2 );
            // Reformat the starting negative number.
            if( startingCoefficient != floor(startingCoefficient) ) {
                // Reformatting for negative fractions.
                toReturn = "(-" + toReturn.substr(1);
            }
            else {
                // Reformatting for negative integers.
                toReturn = "-" + toReturn;
            }
        }
        else {
            // Remove positive sign for the positive payload that begins the string.
            toReturn = toReturn.substr( toReturn.find("+") + 2 );
        }
    }

    // Clip the extra space from the end.
    toReturn = toReturn.substr(0, toReturn.size()-1);

    // If there were no changes made due to the tree or payload being empty, return a "0".
    if(toReturn.empty()) {
        toReturn += "0";
    }

    return toReturn;
}

void ParseAndAdd( string& line, BinaryTree<Payload>& equation ) {
    for(unsigned int i = 0; i < line.size(); i++) {
        char c = line[i];

        // Look for any sign of a term.
        if( c == '-' || (c >= '0' && c <= '9') || c == 'x' ) {
            // Once the start of a term is found,
            // Check if c is not x, to know if we need to find a coefficient or not.
            string coefficient = "";
            if( c != 'x' ) {
                // Get Coefficient
                if(c == '-') {
                    coefficient = '-';
                }
                //
                for( unsigned int j = (c == '-' ? i+1 : i); j < line.size(); j++ ) {
                    if( line[j] >= '0' && line[j] <= '9' ) {
                        // Add the read in single-digit integer to the coefficient string.
                        coefficient += line[j];
                    }
                    else if ( line[j] != ' ' ) {
                        // Move i up to j so that it skips already parsed info.
                        i = j;
                        break;
                    }
                }
            }

            // If there is no coefficient printed, and since the x may have already been found,
            // add a 1 to it if the conditions have been met..
            if(coefficient == "-" || coefficient.empty()) {
                coefficient += "1";
            }

            // If this there is an x following the coefficient,
            // Read in the exponent.
            if( i < line.size() && line[i] == 'x' ) {
                if( i+1 < line.size() && line[i+1] == '^' ) {
                    string exponent = "";
                    if( i+2 < line.size() && line[i+2] == '-' ) {
                        // Add negative sign to the exponent string since it is negative, and move up one i.
                        exponent += '-';
                        i++;
                    }
                    for( unsigned int j = i+2; j < line.size(); j++ ) {
                        if( line[j] >= '0' && line[j] <= '9' ) {
                            // Add a single-digit integer to the exponent string.
                            exponent += line[j];
                        }
                        else {
                            // Since the next character is not an integer, break and move back one i.
                            i = j-1;
                            break;
                        }
                    }
                    // Create the payload by converting the coefficient and exponent strings.
                    Payload payload( stoi(coefficient), stoi(exponent) );
                    /// Add payload to the equation.
                    equation.Add(payload);
                }
                // The exponent is just 1.
                else {
                    // Create the payload by converting the coefficient string,
                    // and create it with a single-exponent x.
                    Payload payload( stoi(coefficient), 1 );
                    /// Add payload to the equation.
                    equation.Add(payload);
                }
            }
            //Otherwise, this is just a whole number.
            else {
                // Create the payload by converting the coefficient string,
                // and create it as a standalone whole-integer.
                Payload payload( stoi(coefficient), 0 );
                /// Add payload to the equation.
                equation.Add(payload);
                i--;
            }
        }
        // If the end of the line is reached,
        else if(i+1 < line.size() && line[i] == 'd' && line[i+1] == 'x') {
                break; // break and return.
        }
    }
}

void IntegrateTree( BinaryTree<Payload>& equation ) {
    // For each item in the Binary Tree.
    for( int i = 0; i < equation.Size(); i++ ) {
        // Pull a reference to the corresponding
        // Payload Object out of the Binary Tree.
        Payload& term = equation.GetAt(i);

        // Integrate.
        term.SetExponent( term.GetExponent()+1 );
        term.SetDenominator( term.GetDenominator() * term.GetExponent() );
    }
}

string EvaluateIntegral( BinaryTree<Payload>& equation, int a, int b ) {
    stringstream stream;
    // Format equation accordingly.
    stream << PrintEquation(equation) << ", ";
    stream << a << "|" << b << " = ";
    double evaluation = 0.0;
    // Pull out each individual payload in the equation,
    // and add its evaluation of the integral to the evaluation total.
    for( int i = 0; i < equation.Size(); i++ ) {
        Payload term = equation.GetAt(i);
        evaluation += (term.Evaluate(b) - term.Evaluate(a));
    }
    // Set precision to round to the nearest 3 decimal places.
    stream.precision(3);
    stream << fixed << evaluation;
    return stream.str();
}

int main()
{
    // Read in the name of the input file.
    string InputFileName;
    cin >> InputFileName;
    // Create file stream.
    ifstream input(InputFileName);

    /// Parse through data in the file stream.
    while( !input.eof() ) {
        // Create needed variables.
        BinaryTree<Payload> equation;
        bool definite;
        int a, b; // only used if the integral is definite.

        // Read in entire line.
        string line;
        getline(inputexe, line);
        // Skip if line is empty.
        if( line.empty() ) {continue;}

        // Read in integration sign
        if( line.find("|") == 0 ) {
            /// Indefinite Integral
            definite = false;
            // Remove integral sign from the line string.
            line = line.substr(line.find("|")+1);
        }
        else {
            /// Definite Integral
            definite = true;
            string aString = line.substr(0, line.find("|"));
            string bString = line.substr(line.find("|")+1, line.find(" "));
            // Convert strings to integers, and save as the a & b values.
            a = stoi(aString);
            b = stoi(bString);
            // Remove integral sign and a & b values from the line string.
            line = line.substr(line.find(" ")+1);
        }

        // Separate each term in the equation.
        ParseAndAdd(line, equation);

        // At this point, each Payload for each corresponding
        // term should be created & added into the Binary Tree.

        /// Integrate the tree.
        IntegrateTree(equation);

        if(definite) {
            /// Evaluate the integral.
            string evaluation = EvaluateIntegral(equation, a, b);
            /// Print evaluated integral, correctly formatted.
            cout << evaluation << endl;
        }
        else {
            /// Print current tree, with "+ C" added to the end.
            cout << PrintEquation(equation) << " + C" << endl;
        }
    }
    // Close input.
    input.close();
    ///
    /// Setup DEBUG Code
    /*
    // Binary Tree/Payload Interaction
    BinaryTree<Payload> tree;
    tree.Add(Payload(1, -5));
    tree.Add(Payload(1, 0));
    tree.Add(Payload(4, 3));
    tree.Add(Payload(3, -2));
    tree.Add(Payload(0, -6));
    tree.Add(Payload(10, 1));
    cout << PrintEquation(tree) << endl;
    cout << "Height: " << tree.Height() << endl;
    cout << endl;
    tree = IntegrateTree(tree);
    if( tree.Size() != 1 && tree.GetAt(0).GetCoefficientAsDouble() != 0 ) {
        cout << PrintEquation(tree) << " + C" << endl;
    }
    else {
        cout << "C" << endl;
    }
    cout << "Height: " << tree.Height() << endl;
    cout << endl;
    */
    ///
    /*
    // Payload Evaluation;
    BinaryTree<Payload> equation;

    int a = 1, b = 4;

    Payload payload1(3, 2, 2);
    Payload payload2(4, 1);
    Payload payload3(-1, 1, -1);

    equation.Add(payload3);
    equation.Add(payload1);
    equation.Add(payload2);


    int a = -2, b = 2;

    Payload payload1(1, 4, 4);
    Payload payload2(-2, 2);

    equation.Add(payload1);
    equation.Add(payload2);

    cout << EvaluateIntegral(equation, a, b) << endl;
    */
    ///
    /*
    // Binary Tree Structure
    BinaryTree<int> tree;
    tree.Add(0);
    tree.Add(-50);
    tree.Add(-25);
    tree.Add(-63);
    tree.Add(325);
    tree.Add(126);
    tree.Add(130);
    tree.Add(120);
    tree.Add(690);
    cout << "Searching for -25: "  << tree.Search(-25) << endl;
    cout << "Searching for 69: " << tree.Search(69) << endl;
    cout << endl;
    cout << "Pre Order: " << tree.GetPreOrder() << endl;
    cout << "In Order: " << tree.GetInOrder() << endl;
    cout << "Reverse In Order: " << tree.GetReverseInOrder() << endl;
    cout << "Post Order: " << tree.GetPostOrder() << endl;
    cout << endl;
    cout << tree << endl;
    cout << endl;
    for( unsigned int i = 0; i < tree.Size(); i++ ) {
        cout << "Value at Index " << i << ": " << tree.GetAt(i) << endl;
    }
    cout << endl;
    */
    ///
}
