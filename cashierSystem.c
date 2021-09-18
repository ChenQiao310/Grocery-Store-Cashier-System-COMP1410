#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef struct Category{
	int code;
	char categoryName[18];
} category;

typedef struct NamePrice{
	int code; 
	float price;
	char name[18];
	struct NamePrice *next;
} namePrice;

typedef struct Transaction{
	int code;
	int number;
	struct Transaction *next;
} transaction;

typedef struct ItemStruct{
	int ItemCode;
	int NumItems;
	float ItemPrice;
	float TotSales;
	char ItemName[18];
	struct ItemStruct *nextItem;
} item;

typedef struct inventoryReport{
	int code;
	char ItemName[18];
	int NumItems;
	float TotSales;
	struct inventoryReport *next;
} inventory;

typedef struct inventoryDat{
	int code;
	char ItemName[18];
	int NumItems;
	float TotSales;
} dat;

typedef struct dailySummary{
	int code;
	char categoryName[18];
	int totItem;
	float totRev;
	struct inventoryReport *inventory;
} summary;

void sortTransaction(transaction *head);

void clearNamePrice(namePrice **head);

void clearTransaction(transaction **head);

void clearItemStruct(item **head);

void clearInventory(inventory **head);
	
int main(){
	//input Use-Case 1
	FILE *fptr=NULL;
	fptr=fopen("CategoryName.txt", "r");
	category categoryName[8];

	if(fptr==NULL){
		puts("Error! File can not be read!");
		exit(1);
	}
	for(int i=0; i<8; i++){
		fscanf(fptr, "%d %[^\n]", &categoryName[i].code, categoryName[i].categoryName);
	}
	fclose(fptr);
	
	int index;
	char name[17];
	for(int i=0; i<8; i++){
		index=0;
		for(int j=0; j<17; j++){
			if(isalpha(*(categoryName[i].categoryName+j))!=0){
				name[index]=*(categoryName[i].categoryName+j);
				index++;
			}
			if(isblank(*(categoryName[i].categoryName+j))!=0&&isalpha(*(categoryName[i].categoryName+j+1))!=0){ 
				name[index]=*(categoryName[i].categoryName+j);
				index++;
			}
		}
		name[index]='\0';
		strcpy(categoryName[i].categoryName, name);
	}
		
	//input Use-Case 2
	fptr=fopen("CodeNamePrice.txt", "r");
	namePrice *headOfNamePrice=NULL;
	namePrice *currentNP;
	namePrice *nnodeNP;
	if(fptr==NULL){
		puts("Error! File can not be read!");
		exit(1);
	}
	while(!feof(fptr)){
		nnodeNP=(namePrice *)malloc(sizeof(namePrice));
		fscanf(fptr, "%d %[^0-9] %f", &(nnodeNP->code), nnodeNP->name, &(nnodeNP->price));
		nnodeNP->next=NULL;
		
		if(nnodeNP->code<100||nnodeNP->code>888)
			continue;
		
		if(headOfNamePrice==NULL){
			headOfNamePrice=nnodeNP;
		}
		else{
			currentNP=headOfNamePrice;
			while(currentNP->next!=NULL){
				currentNP=currentNP->next;
			}
			currentNP->next=nnodeNP;
		}
	}
	fclose(fptr);
	
	currentNP=headOfNamePrice;
	while(currentNP!=NULL){
		index=0;
		for(int j=0; j<17; j++){
			if(isalpha(*(currentNP->name+j))!=0){
				name[index]=*(currentNP->name+j);
				index++;
			}
			if(isblank(*(currentNP->name+j))!=0&&isalpha(*(currentNP->name+j+1))!=0){ 
				name[index]=*(currentNP->name+j);
				index++;
			}
		}
		name[index]='\0';
		strcpy(currentNP->name, name);
		currentNP=currentNP->next;
	}

	//genarate original structure of Use-Case 5
	inventory *headOfInventory=NULL;
	inventory *currentIV;
	inventory *nnodeIV;
	
	currentNP=headOfNamePrice;
	while(currentNP!=NULL){
		nnodeIV=(inventory *)malloc(sizeof(inventory));
		nnodeIV->code=currentNP->code;
		strcpy(nnodeIV->ItemName, currentNP->name);
		nnodeIV->NumItems=0;
		nnodeIV->TotSales=0;
		nnodeIV->next=NULL;
		
		if(headOfInventory==NULL){
			headOfInventory=nnodeIV;
		}
		else{
			currentIV=headOfInventory;
			while(currentIV->next!=NULL){
				currentIV=currentIV->next;
			}
		currentIV->next=nnodeIV;
	    }
		currentNP=currentNP->next;
	}
		
	//input Use-Case 3
	int endOfDay;
	int newCustomer;
	int customer=0;
	
	transaction *headOfTransaction=NULL;
	transaction *currentT;
	transaction *nnodeT;
	
	item *headOfItems=NULL;
	item *currentI;
	item *nnodeI;
	
	while(1){
	printf("\nIs this the end of the day? Enter 1 for YES, 0 for NO: ");
	scanf("%d", &endOfDay);
	if(endOfDay)
		break;
	printf("Input items for new customer? Enter 1 for YES, 0 for NO: ");
	scanf("%d", &newCustomer);
	if(!newCustomer)
		continue;
	customer++;
	
	fptr=fopen("DailyTransactions.txt", "r");
	if(fptr==NULL){
		puts("Error! File can not be read!");
		exit(1);
	}
	while(1){
		nnodeT=(transaction *)malloc(sizeof(transaction));
		fscanf(fptr, "%d %d", &(nnodeT->code), &(nnodeT->number));
		nnodeT->next=NULL;
		if(nnodeT->code==000)
			break;
		
		if(headOfTransaction==NULL){
			headOfTransaction=nnodeT;
		}
		else{
			currentT=headOfTransaction;
			while(currentT->next!=NULL){
				currentT=currentT->next;
			}
			currentT->next=nnodeT;
		}
		sortTransaction(headOfTransaction);
	}
	fclose(fptr);
	
	//generate Use-Case 4 and output
	int totalNum=0;
	float totalSales=0; 
	currentT=headOfTransaction;
	while(currentT!=NULL){
		nnodeI=(item *)malloc(sizeof(item));
		nnodeI->ItemCode=currentT->code;
		nnodeI->NumItems=currentT->number;
		currentNP=headOfNamePrice;
		while(currentNP!=NULL){
			if(currentNP->code==nnodeI->ItemCode){
				nnodeI->ItemPrice=currentNP->price;
				strcpy(nnodeI->ItemName, currentNP->name);
			}
			currentNP=currentNP->next;
		}
		nnodeI->TotSales=(nnodeI->NumItems)*(nnodeI->ItemPrice);
		nnodeI->nextItem=NULL;
		
		//update Use-Case 5
		currentIV=headOfInventory;
		while(currentIV!=NULL && currentIV->code!=nnodeI->ItemCode){
			currentIV=currentIV->next;
		}
		if(currentIV->code==nnodeI->ItemCode){
			currentIV->NumItems+=nnodeI->NumItems;
			currentIV->TotSales+=nnodeI->TotSales;
		}
		//end of update Use-Case 5	
		
		if(headOfItems==NULL){
			headOfItems=nnodeI;
		}
		else{
			currentI=headOfItems;
			while(currentI->nextItem!=NULL){
				currentI=currentI->nextItem;
			}
		currentI->nextItem=nnodeI;
	    }
		
		totalNum+=nnodeI->NumItems;
		totalSales+=nnodeI->TotSales;
		currentT=currentT->next;
	}
	
	printf("\n\nCustomer Receipt # %d\n", customer);
	printf("%s%15s%13s%10s%15s\n", "Code", "Item Name", "Price", "NumItem", "TotSale");
	currentI=headOfItems;
	while(currentI!=NULL){
	printf(" %-9d%-17s%5.2f%10d%15.2f\n", currentI->ItemCode, currentI->ItemName, currentI->ItemPrice, currentI->NumItems, currentI->TotSales);
	currentI=currentI->nextItem;
	}
	printf("\n%37s%5d%15.2f\n\n", "Totals:", totalNum, totalSales);
	
	clearTransaction(&headOfTransaction);
	clearItemStruct(&headOfItems);
	}
	
	//generate original structure of Use-Case 6
	summary summaryReport[8];
	for(int i=0; i<8; i++){
		summaryReport[i].code=categoryName[i].code;
		strcpy(summaryReport[i].categoryName, categoryName[i].categoryName);
		summaryReport[i].totItem=0;
		summaryReport[i].totRev=0.0;
		
		currentIV=headOfInventory;
		while(currentIV->code/100-1<i){
			currentIV=currentIV->next;
		}
		if(currentIV->code/100-1==i){
			summaryReport[i].inventory=currentIV;
		}
		else
			summaryReport[i].inventory=NULL;

	}


	//modify structure of Use-Case 5 
	for(int i=0; i<8; i++){
		if(summaryReport[i].inventory!=NULL){
			currentIV=summaryReport[i].inventory;
			while(currentIV->next!=NULL && currentIV->next->code/100-1==i){
				currentIV=currentIV->next;
			}
		    currentIV->next=NULL;
		}	
	}
	puts("");
	
	//generate report of Use-Case 5 to Standard Output
	int totalNum=0;
	float totalSales=0; 
	puts("**********************************************************");
	puts("");
	for(int i=0; i<8; i++){
		printf("Category Name: %s\nCategory Code: %d\n", summaryReport[i].categoryName, summaryReport[i].code);
		printf("%s%15s%16s%12s\n", "Code", "Item Name", "NumItems", "TotSale");
		
		totalNum=0;
	    totalSales=0; 
		currentIV=summaryReport[i].inventory;
		while(currentIV!=NULL){
			printf(" %-9d%-17s%6d%14.2f\n", currentIV->code, currentIV->ItemName, currentIV->NumItems, currentIV->TotSales);
			totalNum+=currentIV->NumItems;
			totalSales+=currentIV->TotSales;
			currentIV=currentIV->next;
		}
		//update Use-Case 6
		summaryReport[i].totItem=totalNum;
		summaryReport[i].totRev=totalSales;
		//end of update Use-Case 6
		printf("\nTotal Items Sold:%6d\nTotal Sales:%11.2f\n\n\n", totalNum, totalSales);
	}
	
	//generate report of Use-Case 6 to Standard Output
	totalNum=0;
	totalSales=0; 
	puts("**********************************************************");
	puts("Daily Summary Report");
	printf("%s%19s%18s%17s\n", "Code", "Category Name", "#Items Sold", "Tot Sales Amt");
	for(int i=0; i<8; i++){
		printf(" %-9d%-20s%8d%16.2f\n\n", summaryReport[i].code, summaryReport[i].categoryName, summaryReport[i].totItem, summaryReport[i].totRev);
		totalNum+=summaryReport[i].totItem;
		totalSales+=summaryReport[i].totRev;
	}
	printf("\nTotal Customers%12d\nTotal Items Sold%11d\nTotal Sales%16.2f\n", customer, totalNum, totalSales);
	puts("**********************************************************");
	
	//generate binary files of Use-Case 5
	dat data;
	fptr=fopen("Inventory100.dat", "wb");
	if(fptr==NULL){
		puts("Error! File can not be write!");
		exit(1);
	}
	currentIV=summaryReport[0].inventory;
	while(currentIV!=NULL){
		data.code=currentIV->code;
		strcpy(data.ItemName, currentIV->ItemName);
	    data.NumItems=currentIV->NumItems;
		data.TotSales=currentIV->TotSales;
		fwrite(&data, sizeof(dat), 1, fptr);
		currentIV=currentIV->next;
	}
	fclose(fptr);
	
	fptr=fopen("Inventory200.dat", "wb");
	if(fptr==NULL){
		puts("Error! File can not be write!");
		exit(1);
	}
	currentIV=summaryReport[1].inventory;
	while(currentIV!=NULL){
		data.code=currentIV->code;
		strcpy(data.ItemName, currentIV->ItemName);
	    data.NumItems=currentIV->NumItems;
		data.TotSales=currentIV->TotSales;
		fwrite(&data, sizeof(dat), 1, fptr);
		currentIV=currentIV->next;
	}
	fclose(fptr);
	
	fptr=fopen("Inventory300.dat", "wb");
	if(fptr==NULL){
		puts("Error! File can not be write!");
		exit(1);
	}
	currentIV=summaryReport[2].inventory;
	while(currentIV!=NULL){
		data.code=currentIV->code;
		strcpy(data.ItemName, currentIV->ItemName);
	    data.NumItems=currentIV->NumItems;
		data.TotSales=currentIV->TotSales;
		fwrite(&data, sizeof(dat), 1, fptr);
		currentIV=currentIV->next;
	}
	fclose(fptr);
	
	fptr=fopen("Inventory400.dat", "wb");
	if(fptr==NULL){
		puts("Error! File can not be write!");
		exit(1);
	}
	currentIV=summaryReport[3].inventory;
	while(currentIV!=NULL){
		data.code=currentIV->code;
		strcpy(data.ItemName, currentIV->ItemName);
	    data.NumItems=currentIV->NumItems;
		data.TotSales=currentIV->TotSales;
		fwrite(&data, sizeof(dat), 1, fptr);
		currentIV=currentIV->next;
	}
	fclose(fptr);
	
	fptr=fopen("Inventory500.dat", "wb");
	if(fptr==NULL){
		puts("Error! File can not be write!");
		exit(1);
	}
	currentIV=summaryReport[4].inventory;
	while(currentIV!=NULL){
		data.code=currentIV->code;
		strcpy(data.ItemName, currentIV->ItemName);
	    data.NumItems=currentIV->NumItems;
		data.TotSales=currentIV->TotSales;
		fwrite(&data, sizeof(dat), 1, fptr);
		currentIV=currentIV->next;
	}
	fclose(fptr);
	
	fptr=fopen("Inventory600.dat", "wb");
	if(fptr==NULL){
		puts("Error! File can not be write!");
		exit(1);
	}
	currentIV=summaryReport[5].inventory;
	while(currentIV!=NULL){
		data.code=currentIV->code;
		strcpy(data.ItemName, currentIV->ItemName);
	    data.NumItems=currentIV->NumItems;
		data.TotSales=currentIV->TotSales;
		fwrite(&data, sizeof(dat), 1, fptr);
		currentIV=currentIV->next;
	}
	fclose(fptr);
	
	fptr=fopen("Inventory700.dat", "wb");
	if(fptr==NULL){
		puts("Error! File can not be write!");
		exit(1);
	}
	currentIV=summaryReport[6].inventory;
	while(currentIV!=NULL){
		data.code=currentIV->code;
		strcpy(data.ItemName, currentIV->ItemName);
	    data.NumItems=currentIV->NumItems;
		data.TotSales=currentIV->TotSales;
		fwrite(&data, sizeof(dat), 1, fptr);
		currentIV=currentIV->next;
	}
	fclose(fptr);
	
	fptr=fopen("Inventory800.dat", "wb");
	if(fptr==NULL){
		puts("Error! File can not be write!");
		exit(1);
	}
	currentIV=summaryReport[7].inventory;
	while(currentIV!=NULL){
		data.code=currentIV->code;
		strcpy(data.ItemName, currentIV->ItemName);
	    data.NumItems=currentIV->NumItems;
		data.TotSales=currentIV->TotSales;
		fwrite(&data, sizeof(dat), 1, fptr);
		currentIV=currentIV->next;
	}
	fclose(fptr);
	
		//generate text files of Use-Case 5
	fptr=fopen("Inventory100.txt", "w");
	if(fptr==NULL){
		puts("Error! File can not be written!");
		exit(1);
	}
	currentIV=summaryReport[0].inventory;
	while(currentIV!=NULL){
		fprintf(fptr, "%d   %s   %d   %.2f\n", currentIV->code, currentIV->ItemName, currentIV->NumItems, currentIV->TotSales);
		currentIV=currentIV->next;
	}
	fclose(fptr);
	
	fptr=fopen("Inventory200.txt", "w");
	if(fptr==NULL){
		puts("Error! File can not be written!");
		exit(1);
	}
	currentIV=summaryReport[1].inventory;
	while(currentIV!=NULL){
		fprintf(fptr, "%d   %s   %d   %.2f\n", currentIV->code, currentIV->ItemName, currentIV->NumItems, currentIV->TotSales);
		currentIV=currentIV->next;
	}
	fclose(fptr);
	
	fptr=fopen("Inventory300.txt", "w");
	if(fptr==NULL){
		puts("Error! File can not be written!");
		exit(1);
	}
	currentIV=summaryReport[2].inventory;
	while(currentIV!=NULL){
		fprintf(fptr, "%d   %s   %d   %.2f\n", currentIV->code, currentIV->ItemName, currentIV->NumItems, currentIV->TotSales);
		currentIV=currentIV->next;
	}
	fclose(fptr);
	
	fptr=fopen("Inventory400.txt", "w");
	if(fptr==NULL){
		puts("Error! File can not be written!");
		exit(1);
	}
	currentIV=summaryReport[3].inventory;
	while(currentIV!=NULL){
		fprintf(fptr, "%d   %s   %d   %.2f\n", currentIV->code, currentIV->ItemName, currentIV->NumItems, currentIV->TotSales);
		currentIV=currentIV->next;
	}
	fclose(fptr);
	
	fptr=fopen("Inventory500.txt", "w");
	if(fptr==NULL){
		puts("Error! File can not be written!");
		exit(1);
	}
	currentIV=summaryReport[4].inventory;
	while(currentIV!=NULL){
		fprintf(fptr, "%d   %s   %d   %.2f\n", currentIV->code, currentIV->ItemName, currentIV->NumItems, currentIV->TotSales);
		currentIV=currentIV->next;
	}
	fclose(fptr);
	
	fptr=fopen("Inventory600.txt", "w");
	if(fptr==NULL){
		puts("Error! File can not be written!");
		exit(1);
	}
	currentIV=summaryReport[5].inventory;
	while(currentIV!=NULL){
		fprintf(fptr, "%d   %s   %d   %.2f\n", currentIV->code, currentIV->ItemName, currentIV->NumItems, currentIV->TotSales);
		currentIV=currentIV->next;
	}
	fclose(fptr);
	
	fptr=fopen("Inventory700.txt", "w");
	if(fptr==NULL){
		puts("Error! File can not be written!");
		exit(1);
	}
	currentIV=summaryReport[6].inventory;
	while(currentIV!=NULL){
		fprintf(fptr, "%d   %s   %d   %.2f\n", currentIV->code, currentIV->ItemName, currentIV->NumItems, currentIV->TotSales);
		currentIV=currentIV->next;
	}
	fclose(fptr);
	
	fptr=fopen("Inventory800.txt", "w");
	if(fptr==NULL){
		puts("Error! File can not be written!");
		exit(1);
	}
	currentIV=summaryReport[7].inventory;
	while(currentIV!=NULL){
		fprintf(fptr, "%d   %s   %d   %.2f\n", currentIV->code, currentIV->ItemName, currentIV->NumItems, currentIV->TotSales);
		currentIV=currentIV->next;
	}
	fclose(fptr);
	
	//deallocate all dynamic memory allocations
	clearNamePrice(&headOfNamePrice);
	clearTransaction(&headOfTransaction);
	clearItemStruct(&headOfItems);
	for(int i=0; i<8; i++){
		clearInventory(&(summaryReport[i].inventory));
	}
	
	return 0;
}

void sortTransaction(transaction *head){
	transaction *current;
	transaction *after;
	transaction temp;
	
	for(current=head; current->next!=NULL; current=current->next){
		for(after=current->next; after!=NULL; after=after->next){
			if(current->code > after->code){
				temp.code=current->code;
				temp.number=current->number;
				current->code=after->code;
				current->number=after->number;
				after->code=temp.code;
				after->number=temp.number;
			}
		}
	}
}

void clearNamePrice(namePrice **head){
	namePrice *current=*head;
	while(current!=NULL){
		current=current->next;
		free(*head);
		*head=current;
	}
}

void clearTransaction(transaction **head){
	transaction *current=*head;
	while(current!=NULL){
		current=current->next;
		free(*head);
		*head=current;
	}
}

void clearItemStruct(item **head){
	item *current=*head;
	while(current!=NULL){
		current=current->nextItem;
		free(*head);
		*head=current;	
	}
}

void clearInventory(inventory **head){
	inventory *current=*head;
	while(current!=NULL){
		current=current->next;
		free(*head);
		*head=current;
	}
}


