#include <iostream>
#include <math.h>
#include <fstream>
using std::ofstream;
using namespace std;

int main() {

    while(true){

        // Variables to store customer details
        string fullName;
        int age;
        double monthlyIncome;

        // Arrays to store loans details
        string LOAN_NAMES[6] = {"Vehicle","Housing","Education","Senior Citizen","Personal","Personal"};
        int LOWER_AGE_LIMITS[6] = {18,25,25,60,30,30};
        int HIGHER_AGE_LIMITS[6] = {55,55,35,150,55,55};
        double MONTHLY_INCOME[6] = {45000.00,100000.00,45000.00,35000.00,100000.00,100000.00};
        double MAX_LOAN_AMOUNT[6] = {1000000.00,2500000.00,1500000.00,500000.00,2000000.00,3000000.00};
        int MAX_REPAY_PERIOD[6] = {60,60,84,60,60,84};
        float ANNUAL_INTEREST_RATE[6] = {14,8,6,4.5,14.5,16.2};

        cout<<"*********************************************************"<<endl;
        cout<<"********** BANK LOAN MONTHLY PAYMENT SYSTEM *************"<<endl;
        cout<<"*********************************************************"<<endl;

        // Get the customer's details
        cout<<"Please enter full name :: ";
        getline(cin,fullName);
        cout<<"Please enter age :: ";
        cin>>age;
        cout<<"Please enter monthly income :: ";
        cin>>monthlyIncome;

        // Display loans that customer can apply
        int numberOfPossibleLoans = 0 ;
        int possibleLoans[6];
        cout<<"You can apply for following loans : - "<<endl;
        cout<<"_____________________________________________________"<<endl;
        for(int i=0;i<6;i++){
            if(age>=LOWER_AGE_LIMITS[i] && age<=HIGHER_AGE_LIMITS[i] && monthlyIncome >= MONTHLY_INCOME[i]){
                possibleLoans[numberOfPossibleLoans] = i;
                numberOfPossibleLoans++;
                cout<<"Loan Type Number : "<<i<<endl;
                cout<<"Name of the Loan : "<<LOAN_NAMES[i]<<endl;
                cout<<"Maximum Loan Amount : (LKR) "<<fixed<<MAX_LOAN_AMOUNT[i]<<endl;
                cout<<"Maximum Repay Period Allowed : "<<MAX_REPAY_PERIOD[i]<<" months"<<endl;
                cout<<"Annual Interest Rate : "<<ANNUAL_INTEREST_RATE[i]<<"%"<<endl;
                cout<<"_____________________________________________________"<<endl;
            }
        }
        cout<<endl;
        if(numberOfPossibleLoans==0){
            cout<<"Sorry , the customer is not suitable to get any loan type!"<<endl;
        }else{
            int selectedLoanType;
            cout<<"Please enter the loan type number :: ";
            cin>>selectedLoanType;

            bool checkValid = false;
            for(int i=0;i<numberOfPossibleLoans;i++){
                if(selectedLoanType==possibleLoans[i]){
                    checkValid=true;
                }
            }

            while((selectedLoanType<0||selectedLoanType>5)||checkValid==false){
                cout<<"Please enter valid loan type number :: ";
                cin>>selectedLoanType;
                checkValid = false;
                for(int i=0;i<numberOfPossibleLoans;i++){
                    if(selectedLoanType==possibleLoans[i]){
                        checkValid=true;
                    }
                }
            }

            double amountOfMoneyToBorrow;
            int possibleRepayTime;

            cout<<"Please enter the amount of money that customer wish to borrow :: (LKR) ";
            cin>>amountOfMoneyToBorrow;
            cout<<"Please enter the possible repay time :: (months) ";
            cin>>possibleRepayTime;

            while(amountOfMoneyToBorrow>MAX_LOAN_AMOUNT[selectedLoanType]){
                cout<<"Please enter a amount less than (LKR) "<<MAX_LOAN_AMOUNT[selectedLoanType]<<"  :: ";
                cin>>amountOfMoneyToBorrow;
            }
            while(possibleRepayTime>MAX_REPAY_PERIOD[selectedLoanType]){
                cout<<"Please enter a period less than "<<MAX_REPAY_PERIOD[selectedLoanType]<<" months :: ";
                cin>>possibleRepayTime;
            }

            // Calculate the discount factor and total monthly payment
            cout<<"Possible Repay Time "<<possibleRepayTime<<endl;
            float interestRate = ANNUAL_INTEREST_RATE[selectedLoanType]/(12*100);
            cout<<"IR "<<interestRate<<endl;
            double discountFactor = (pow(1+interestRate,possibleRepayTime)-1)/(interestRate* pow(1+interestRate,possibleRepayTime));
            cout<<discountFactor;
            double totalMonthlyPayement = amountOfMoneyToBorrow / discountFactor;
            cout<<endl<<endl;
            cout<<"*************************** CONGRATULATIONS ****************************"<<endl;
            cout<<"********************** The customer got the loan ***********************"<<endl;
            cout<<"Monthly Repay Amount     : (LKR) "<<setprecision(2)<<fixed<<totalMonthlyPayement<<endl;
            cout<<"Total Repay Amount       : (LKR) "<<setprecision(2)<<fixed<<totalMonthlyPayement*possibleRepayTime<<endl;
            cout<<"Total Interest           : (LKR) "<<setprecision(2)<<fixed<<(totalMonthlyPayement*possibleRepayTime-amountOfMoneyToBorrow)<<endl;

            ofstream outdata;
            outdata.open("/Users/chanukaabeysinghe/CLionProjects/BankLoanApplication/"+fullName+".txt");
            if(!outdata){
                cout<<"Couldn't open the file"<<endl;
            }else{
                outdata<<"Full Name                 : "<<fullName<<endl;
                outdata<<"Selected Loan             : "<<LOAN_NAMES[selectedLoanType]<<endl;
                outdata<<"Maximum Repay Period      : "<<MAX_REPAY_PERIOD[selectedLoanType]<<" months"<<endl;
                outdata<<"Annual Interest Rate      : "<<ANNUAL_INTEREST_RATE[selectedLoanType]<<" %"<<endl;
                outdata<<"Requested Loan Amount     : (LKR) "<<setprecision(2)<<fixed<<amountOfMoneyToBorrow<<endl;
                outdata<<"Requested Repay Period    : "<<possibleRepayTime<<" months"<<endl;
                outdata<<endl<<endl;
                outdata<<"************************ Monthly Repay Calculation ***************************"<<endl;
                outdata<<"----------------------------------------------------------------"<<endl;
                outdata<<"Month | Remaining Loan Amount | Interest | Repaid Loan Amount  "<<endl;
                outdata<<"----------------------------------------------------------------"<<endl;
                double remainingLoan = amountOfMoneyToBorrow;
                double repaidLoanAmount = 0;
                double totalRepaidAmount =0;
                double interest = 0;
                for(int i=0;i<possibleRepayTime;i++){
                    interest = (remainingLoan)*interestRate;
                    repaidLoanAmount = totalMonthlyPayement - interest;
                    outdata<<i+1<<" | ";
                    outdata<<setprecision(2)<<fixed<<remainingLoan<<" | ";
                    outdata<<setprecision(2)<<fixed<<interest<<" | ";
                    outdata<<setprecision(2)<<fixed<<repaidLoanAmount<<" | ";
                    remainingLoan -=repaidLoanAmount;
                    outdata<<endl;
                    outdata<<"----------------------------------------------------------------"<<endl;
                }
                outdata<<"Total Amount of Money    : "<<setprecision(2)<<fixed<<totalMonthlyPayement*possibleRepayTime<<endl;
            }
        }

        int exitNo;
        cout<<"Please enter 0 if you want to exist :: ";
        cin>>exitNo;
        if(exitNo==0){
            break;
        }
    }
    return 0;
}
