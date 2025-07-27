### 【1820】read csv text file and create points 读取CSV文本文件并创建点

#### 代码

```cpp
    /*HEAD READ_CSV_TEXT_FILE_AND_CREATE_POINTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
    #include <uf_part.h>  
    #include <uf_view.h>  
    #include <uf_curve.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #define ECHO(X) { UF_UI_open_listing_window(); \  
        UF_UI_write_listing_window(X); \  
        UF_print_syslog(X, FALSE); }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[UF_UI_MAX_STRING_LEN];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
            ECHO(msg);  
            ECHO(err);  
            ECHO("\n");  
            ECHO(call);  
            ECHO(";\n");  
        }  
        return(irc);  
    }  
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    static logical prompt_for_csv_text_name(char *prompt, char *fspec)  
    {  
        int  
            resp;  
        char  
            filter[MAX_FSPEC_SIZE+1];  
        strcpy(filter, "*.csv");  
        if (!UF_CALL(UF_UI_create_filebox(prompt, "CSV Text File Name", filter, "",  
            fspec, &resp)) && (resp != UF_UI_CANCEL)) return TRUE;  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            inx = 0,  
            cntr = 1,  
            n_lines = 0;  
        tag_t  
            point = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        double  
             point_location[3] = { 0., 0., 0. };  
        char  
            msg[300] = { "" },  
            *txtptr = NULL,  
            pname[30] = { "" },  
            a_line[MAX_LINE_SIZE+1] = { "" },  
            fname[MAX_FSPEC_SIZE + 1] = { "" };  
        FILE  
            *csv_file = NULL;  
        if( NULL_TAG == part )  
        {  
            sprintf( msg, "No Current Part.  Program needs a part.\n\n" );  
            WRITE( msg );  
            return;  
        }  
        if(prompt_for_csv_text_name("Select CSV Text File:", fname))  
        {  
            csv_file = fopen( fname, "r" );  
            if( csv_file == NULL )  
            {  
                sprintf( msg, "Could not open the file, %s\n", fname);  
                WRITE( msg );  
                return;  
            }  
            else  
            {  
                while ((fgets(a_line, MAX_LINE_SIZE, csv_file)) != NULL) n_lines++;  
                rewind(csv_file);  
                for (inx = 0; inx < n_lines; inx++)  
                {  
                    fgets(a_line, MAX_LINE_SIZE, csv_file);  
                    if( (txtptr = strchr(a_line, ',')) == NULL ) continue;  
                    point_location[0] = atof( a_line );  
                    txtptr += sizeof( char );  
                    point_location[1] = atof( txtptr );  
                    txtptr = strchr(txtptr, ',' ) + sizeof( char );  
                    point_location[2] = atof( txtptr );  
                    UF_CALL( UF_CURVE_create_point( point_location, &point ));  
                    sprintf( pname, "POINT_%d", cntr++ );  
                    /* comment next line to avoid naming the points. 里海译:不要命名点，而是翻译下一行注释。 */  
                    UF_CALL( UF_OBJ_set_name( point, pname ));  
                }  
                fclose( csv_file );  
                UF_VIEW_fit_view( NULL_TAG, 0.9 );  
            }  
         }  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是用于NX的二次开发，其主要功能是读取CSV文件中的坐标点，并在当前部件中创建相应的点。以下是代码的主要功能介绍：
>
> 1. 引入头文件：代码引入了NX提供的头文件，包括UF函数库、对象、建模、部件、视图、曲线等相关头文件。
> 2. 宏定义：定义了错误报告宏和打印宏，用于输出错误信息和提示信息。
> 3. 提示函数：prompt_for_csv_text_name函数用于弹出一个文件选择框，提示用户选择CSV文件。
> 4. 主函数：do_it函数是主函数，用于读取CSV文件，逐行解析坐标点，并调用UF_CURVE_create_point函数在当前部件中创建点，同时可以设置点的名称。
> 5. ufusr函数：这是NX二次开发的标准入口函数，在NX启动时会调用该函数。在函数中初始化UF函数库，调用do_it函数，然后终止UF函数库。
> 6. 卸载函数：ufusr_ask_unload函数用于处理模块卸载时的行为。
>
> 总体来说，这段代码实现了读取CSV文件并创建点的功能，通过NX提供的UF函数库实现了二次开发。
>
