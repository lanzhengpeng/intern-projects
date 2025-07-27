### 【0325】ask part type 该程序名为ask_part_type，主要用于询问和判断NX部件文件类型

#### 代码

```cpp
    /*****************************************************************************  
    **  
    ** ask_part_type.c  
    **  
    ** Description:  
    **    Main file for the application.  
    **  
    *****************************************************************************/  
    /* Include files 译:Include files翻译为包含文件。 */  
    #include <stdio.h>  
    #include <uf.h>  
    #include <uf_obj.h>  
    #include <uf_part.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
            UF_print_syslog(msg, FALSE);  
            UF_print_syslog(err, FALSE);  
            UF_print_syslog("\n", FALSE);  
            UF_print_syslog(call, FALSE);  
            UF_print_syslog(";\n", FALSE);  
        }  
        return(irc);  
    }  
    void report_load_status_extern(UF_PART_load_status_p_t status)  
    {  
        char  
            msg[133];  
        int  
            ii;  
        if (status->n_parts > 0)  
        {  
            printf("\tLoad notes:\n");  
            for (ii = 0; ii < status->n_parts; ii++)  
            {  
                printf("\t%s\n", status->file_names[ii]);  
                UF_get_fail_message(status->statuses[ii], msg);  
                printf("\t%s\n\n", msg);  
            }  
            UF_free(status->statuses);  
            UF_free_string_array(status->n_parts, status->file_names);  
        }  
    }  
    /* Main routine 译:Main routine的翻译是：主程序。 */  
    extern int main( char argc, char *argv[] )  
    {  
        int parttype = 1;  
        tag_t   
            part = NULL_TAG,  
            obj = NULL_TAG;  
        UF_PART_load_status_t  
            status;  
        logical  
            dwg_found = FALSE,  
            assy_found = FALSE;  
        /* Initialize the API environment 译:初始化API环境 */  
        if( UF_CALL(UF_initialize()) )   
        {  
            /* Failed to initialize 译:Failed to initialize 的中文翻译是“初始化失败”。 */  
            printf("\nFailed to initialize, do you have a valid license?\n");  
            return 8;  
        }  
        /* TODO: Add your application code here 译:翻译为：TODO：在此处添加你的应用程序代码 */  
        if( argc < 2)  
        {  
            printf("\nUsage: program.exe Partfile\n");  
            printf("Returns the following values which can be used by ERRORLEVEL:\n");  
            printf("1 = simple\n");  
            printf("3 = drawing\n");  
            printf("4 = assembly\n");  
            printf("6 = drawing AND assembly (Master-Model)\n");  
            printf("7 = no partfile passed as argument\n");  
            printf("8 = no license\n");  
            return 7;  
        }  
        UF_CALL(UF_PART_open(argv[1], &part, &status));  
        if (status.n_parts > 0) report_load_status_extern(&status);  
        if (!status.failed && part != NULL_TAG)  
        {  
            // Cycle all drawings and quit if one is found  
            obj = NULL_TAG;  
            while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_drawing_type, &obj)) && (obj != NULL_TAG))  
            {  
                dwg_found = TRUE;  
                break;  
            }  
            // Cycle all components and quit if one is found  
            obj = NULL_TAG;  
            while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_component_type, &obj)) && (obj != NULL_TAG))  
            {  
                assy_found = TRUE;  
                break;  
            }  
        }  
        UF_CALL( UF_PART_close(part, 1, 0) );  
        /* Terminate the API environment 译:终止API环境 */  
        UF_CALL(UF_terminate());  
        if( dwg_found ) parttype += 2;  
        if( assy_found ) parttype += 3;  
        printf("\nPart Type: %d\n", parttype);  
        return parttype;  
    }

```

#### 代码解析

> 这段代码是用于判断NX部件类型的二次开发代码。主要功能包括：
>
> 1. 初始化和终止NX API环境。
> 2. 打开指定的NX部件文件，并判断其类型。
> 3. 遍历部件中的对象，判断是否包含图纸或装配。
> 4. 根据部件是否包含图纸和装配，输出不同的类型码。
> 5. 输出类型码，其中1表示简单零件，3表示图纸，4表示装配，6表示图纸和装配，7表示未指定部件文件，8表示无许可。
>
> 代码中使用了NX提供的API函数，通过遍历部件中的对象类型来判断是否包含图纸或装配。最终输出部件的类型码，供外部调用。
>
> 总体来说，这段代码实现了对NX部件类型的判断，具有二次开发的应用场景。
>
