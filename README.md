# DNA_Protein_Transcription

Program for Translation from DNA sequence to protein sequence

Language: C

Neelotpal Dutta and Anirudh Nistala 

Date: November, 2016


## Overview:
This program translates DNA sequences present in an input file to their corresponding protein sequences and their respective lengths which are printed in another external file.


## Input:
->The DNA sequences must be present in a file, in either a Multi Fasta or a Uni Fasta format,the address of which  must be passed to the program via a shell command at the time of execution.

->The input file must contain no more than a 1000 sequences keeping in view the storage capabilities of the CPU.

## Translation:
->Every DNA sequence is translated into 6 different frames by convention(3 forward frames and 3 backward frames)
->Translation of codons to their corresponding amino acid takes place in reference to the standard codon table.
(to see the codon table visit (www.winstudent.com/the-genetic-code/) )

->The symbol ‘X’ in the protein sequence stands for any codon which doesn’t have a corresponding amino acid according to the codon table. Symbol ’M’ stand for the START codon  whereas the symbol ’#’ stands for a STOP codon . 

## Output:
->The name of an external file in which you desire to view the translated protein sequences must also be   passed to the program through a shell command.


## Sample shell command:
`$ myprog  inputfile.txt  outputfile.txt`

**Check the number of sequences in the input file before using it**

If the user does not enter any output file, the program considers a default output file.

THE PROGRAM IGNORES ANY CHARACTER OTHER  THAN ALPHABETS IN THE SEQUENCE
