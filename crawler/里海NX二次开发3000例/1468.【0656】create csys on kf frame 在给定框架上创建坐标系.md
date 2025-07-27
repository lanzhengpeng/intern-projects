### 【0656】create csys on kf frame 在给定框架上创建坐标系

#### 代码

```cpp
    /*HEAD CREATE_CSYS_ON_KF_FRAME CCC UFUN */  
    /*  
        The corresponding dfa file defining the KF function would look like  
        this:  
        #! UGNX/KF 2.0  
        #+  
            This function will create a coordinate system on the given frame  
            Input :  a frame  
            Return:  the csys hostpointer  
        #-  
        Defun: create_csysOnFrame (Frame $frame,  
                       String ($libname, "/path/to/this.sl"),  
                       String ($funcname, "create_csysOnFrame") )  
        @{ Cfunc("UF_KF_invoke_user_function"; "libufun"); } Any ;  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_kf.h>  
    #include <uf_error_bases.h>  
    #include <uf_mtx.h>  
    #include <uf_csys.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译：

注意：UF_print_syslog是在V18版本中新增的。 */  
            UF_print_syslog(msg, FALSE);  
            UF_print_syslog(err, FALSE);  
            UF_print_syslog("\n", FALSE);  
            UF_print_syslog(call, FALSE);  
            UF_print_syslog(";\n", FALSE);  
            if (!UF_UI_open_listing_window())  
            {  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window(err);  
                UF_UI_write_listing_window("\n");  
                UF_UI_write_listing_window(call);  
                UF_UI_write_listing_window(";\n");  
            }  
        }  
        return(irc);  
    }  
    /* 里海 */  
    extern DllExport void create_csysOnFrame(  
        UF_KF_value_t *data,  
        int num,  
        UF_KF_value_t *rv)  
    {  
        tag_t  
            csys,  
            matrix;  
        double  
            mtx3[9],  
            mtx4[16],  
            vec[3];  
        *rv = NULL;  
        if (UF_CALL(UF_initialize())) return;  
        if (UF_CALL(UF_KF_ask_frame(data[0], mtx4)))  
        {  
            UF_CALL(UF_KF_make_error(UF_err_bad_parameter_number_1, rv));  
            UF_terminate();  
            return;  
        }  
        UF_MTX4_ask_rotation(mtx4, mtx3);  
        UF_MTX4_ask_translation(mtx4, vec);  
        if (!UF_CALL(UF_CSYS_create_matrix(mtx3, &matrix)) &&  
            !UF_CALL(UF_CSYS_create_csys(vec, matrix, &csys)))  
            UF_CALL(UF_KF_make_tag(csys, rv));  
        else  
            UF_CALL(UF_KF_make_error(UF_err_operation_aborted, rv));  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是一个NX Open API的二次开发代码，用于在NX的KF框架上创建坐标系。具体来说：
>
> 1. 首先，定义了报告错误函数report_error，用于打印出调用函数时产生的错误代码和消息。
> 2. 然后，定义了主函数create_csysOnFrame，用于接收一个frame参数，并在该frame上创建一个坐标系。主要步骤包括：调用UF_KF_ask_frame获取frame的4x4变换矩阵。提取变换矩阵中的旋转矩阵和位移向量。调用UF_CSYS_create_matrix和UF_CSYS_create_csys，利用旋转矩阵和位移向量创建坐标系。最后，将创建的坐标系作为返回值。
> 3. 调用UF_KF_ask_frame获取frame的4x4变换矩阵。
> 4. 提取变换矩阵中的旋转矩阵和位移向量。
> 5. 调用UF_CSYS_create_matrix和UF_CSYS_create_csys，利用旋转矩阵和位移向量创建坐标系。
> 6. 最后，将创建的坐标系作为返回值。
> 7. 最后，定义了卸载函数ufusr_ask_unload，返回立即卸载。
>
> 总的来说，这段代码实现了在给定frame上创建坐标系的功能，并使用了NX Open API中的UF模块和KF框架。代码结构清晰，调用API的顺序合理。
>
