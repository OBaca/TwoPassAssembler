
/*insert_funct, insert the funct value to the structure */
void insert_funct(int, line *);

/*insert_address, insert the adress value to the structure depends if its a source or destination */
void insert_address(int ,int, line *);

/*insert_register, insert the register value to the structure depends if its a source or destination */
void insert_register(int, int, line *);

/*insert_claassify, turning on the classify flag. */
void insert_classify(int, line *);

void insert_opcode(int , line *);

void print_obj_line(FILE *,line *, int );

line *create_single_line();

