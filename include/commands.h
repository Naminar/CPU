
DEF_CMD(PUSH, 1,
{
    argument_reader(&ip, code, CMD_PUSH, read_file, out_file, 1);
},


	switch(cmd_key)
	{
        case -1:
        {
            stack_push(&cpu.STK, CELL_VALUE(DATA, cpu.code));

            //ip += sizeof(DATA);

            break;
        }

        case 0:
        {
            stack_push(&cpu.STK, cpu.RAM[(int) CELL_VALUE(DATA, cpu.code)]);///////////must change

            //ip += sizeof(DATA);

            break;
        }


        case 1:
        {
            stack_push(&cpu.STK, cpu.REG[(int) CELL_VALUE(DATA, cpu.code)]);

            //ip += sizeof(DATA);

            break;
        }


        case 2:
        {
            stack_push(&cpu.STK, cpu.RAM[(int) cpu.REG[(int) CELL_VALUE(DATA, cpu.code)]]);///////////must change

            printf(ELEMENT_FMT"2",CELL_VALUE(DATA, cpu.code));

            //ip += sizeof(DATA);

            break;
        }

        default:
        {
            printf("PROGRAM BROKE! SEND SOME MONEY FOR ME...");

            break;
        }

    }

    IP_UP(sizeof(DATA))
)

DEF_CMD(OUT,  2,
{
    argument_reader(&ip, code, CMD_OUT, read_file, out_file, 0);
},


    static DATA element = 0;

	stack_pop(&cpu.STK, &element);

	printf(ELEMENT_FMT"\n", element);
)

DEF_CMD(MUL,  3,
{
    argument_reader(&ip, code, CMD_MUL, read_file, out_file, 0);
},


    static DATA first_element  = 0;
    static DATA second_element = 0;

	stack_pop(&cpu.STK, &first_element);

	stack_pop(&cpu.STK, &second_element);

	stack_push(&cpu.STK, first_element * second_element);
)

DEF_CMD(SUB,  4,
{
    argument_reader(&ip, code, CMD_SUB, read_file, out_file, 0);
},


    static DATA first_element  = 0;
    static DATA second_element = 0;

	stack_pop(&cpu.STK, &first_element);

	stack_pop(&cpu.STK, &second_element);

	stack_push(&cpu.STK, first_element - second_element);
)

DEF_CMD(HLT,  5,
{
    DBG fprintf(out_file, "\n%d", CMD_HLT);

    *(char*) (code + ip) = CMD_HLT;

    IP_UP(sizeof(char))//ip += sizeof(char);
},


	hlt_key = 0;

    printf("hlt");
)

DEF_CMD(ADD,  6,
{
    argument_reader(&ip, code, CMD_ADD, read_file, out_file, 0);
},


    static DATA first_element  = 0;
    static DATA second_element = 0;

	stack_pop(&cpu.STK, &first_element);

	stack_pop(&cpu.STK, &second_element);

	stack_push(&cpu.STK, first_element + second_element);
)


DEF_CMD(POP, 7,
{
    argument_reader(&ip, code, CMD_POP, read_file, out_file, 1);
},


	switch(cmd_key)
	{
        case 0:
        {
            stack_pop(&cpu.STK, &cpu.RAM[(int) CELL_VALUE(DATA, cpu.code)]);///////////must change

            //IP_UP(sizeof(DATA))

            break;
        }


        case 1:
        {
            stack_pop(&cpu.STK, &cpu.REG[(int) CELL_VALUE(DATA, cpu.code)]);

            //IP_UP(sizeof(DATA))

            break;
        }


        case 2:
        {
            stack_pop(&cpu.STK, &cpu.RAM[(int) cpu.REG[(int) CELL_VALUE(DATA, cpu.code)]]);///////////must change

            printf(ELEMENT_FMT"2f_pop",CELL_VALUE(DATA, cpu.code));

            //IP_UP(sizeof(DATA))

            break;
        }

        default:
        {
            printf("POP SYNTAX ERROR");

            ip -= (sizeof(DATA));

            break;
        }

    }

    IP_UP(sizeof(DATA))
)

DEF_CMD(JMP,  8,
{
    argument_reader(&ip, code, CMD_JMP, read_file, out_file, 0);

    fscanf(read_file, "%s",   input_argument);

    int ind = 0;

    while (strcmp(jump_struct.jump_labels[ind].name, input_argument) && ind < jump_struct.capacity)
        ++ind;

    printf("%d", ind);

    if (ind != jump_struct.capacity)
    {
        *(int*) (code + ip) = jump_struct.jump_labels[ind].address;

        printf("!!!!%d!!!!", jump_struct.jump_labels[ind].address);
    }
    else
    {
        do_double_asm_key = 2;
    }

    IP_UP(sizeof(int))//ip += sizeof(int);
},


    ip = CELL_VALUE(int, cpu.code);
)

DEF_JMP(JA,  9,   >)
DEF_JMP(JAL, 10, >=)
DEF_JMP(JB,  11,  <)
DEF_JMP(JBL, 12, <=)
DEF_JMP(JL,  13, ==)
DEF_JMP(JNL, 14, !=)
//===============================================

DEF_CMD(CALL,  15,
{
    argument_reader(&ip, code, CMD_CALL, read_file, out_file, 0);

    fscanf(read_file, "%s",   input_argument);

    int ind = 0;

    while (strcmp(jump_struct.jump_labels[ind].name, input_argument) && ind < jump_struct.size + 1)
        ++ind;

    printf("%d", ind);

    if (ind != jump_struct.size + 1)
    {
        *(int*) (code + ip) = jump_struct.jump_labels[ind].address;

        printf("!!!!%d!!!!", jump_struct.jump_labels[ind].address);
    }
    else
    {
        do_double_asm_key = 2;
    }

    IP_UP(sizeof(int))//ip += sizeof(int);
},


    stack_push(&ptr_stk, ip + sizeof(int));

    ip = CELL_VALUE(int, cpu.code);
)

DEF_CMD(RET,  16,
{
    argument_reader(&ip, code, CMD_RET, read_file, out_file, 0);
},


    static DATA ip_help = 0;

    stack_pop(&ptr_stk, &ip_help);

    ip = (size_t) ip_help;
)

DEF_CMD(PVM,  17,
{
    argument_reader(&ip, code, CMD_PVM, read_file, out_file, 0);

    fscanf(read_file, "%d %d %d %d %d",
    (int*) (code + ip),
    (int*) (code + ip) + 1,
    (int*) (code + ip) + 2,
    (int*) (code + ip) + 3,
    (int*) (code + ip) + 4);

    IP_UP(5 * sizeof(int))//ip += 5 * sizeof(int);
},


    for (int ind = 0; ind < 5; ind++)
    {
        cpu.VIDEO[video_ip++] = CELL_VALUE(int, cpu.code);

        IP_UP(sizeof(int))
    }
)

DEF_CMD(SHOW,  18,
{
    argument_reader(&ip, code, CMD_SHOW, read_file, out_file, 0);
},


    show_command(video_ip, cpu.VIDEO);

    video_ip = 0;
)


DEF_CMD(SQRT,  19,
{
    argument_reader(&ip, code, CMD_SQRT, read_file, out_file, 0);
},


    static DATA first_element  = 0;

	stack_pop(&cpu.STK, &first_element);

	stack_push(&cpu.STK, (DATA) sqrtf(first_element));
)

DEF_CMD(DIV,  20,
{
    argument_reader(&ip, code, CMD_DIV, read_file, out_file, 0);
},


    static DATA first_element  = 0;
    static DATA second_element = 0;

	stack_pop(&cpu.STK, &first_element);

	stack_pop(&cpu.STK, &second_element);

	stack_push(&cpu.STK, (DATA) (first_element / second_element));
)
