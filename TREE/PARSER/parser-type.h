#ifndef PARSER_TYPE_H
#define PARSER_TYPE_H


// -------------------------------------------------------------------------------------------------------
struct comand_t
{
    int hash;
    size_t arg;
};
// -------------------------------------------------------------------------------------------------------
struct file_data_t
{
    char* buffer;
    char** mass_point;
    comand_t* comands;
    size_t amount_line;
    size_t amount_cmd;
};
// -------------------------------------------------------------------------------------------------------
struct comand_inf_t
{
    size_t amount_cmd;
    comand_t* mass_cmd;
};
// -------------------------------------------------------------------------------------------------------

#endif // PARSER_TYPE_H
