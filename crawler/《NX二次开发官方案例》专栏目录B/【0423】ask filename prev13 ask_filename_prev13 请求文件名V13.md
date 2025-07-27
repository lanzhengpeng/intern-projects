### 【0423】ask filename prev13 ask_filename_prev13 请求文件名V13

#### 代码

```cpp
    /*HEAD ASK_FILENAME_PREV13 CCC UFUN */  
    /*  In V13.0 look for a new function UF_UI_create_filebox, it should replace  
        this code entirely. */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <Xm/FileSB.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_ui_xt.h>  
    typedef struct  
    {  
        char  
            cue[81],  
            title[81],  
            directory[133],  
            filter[31],  
            filename[133];  
    } FSB_data_t;  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog 是 V18 中的新增功能。 */  
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
    static void file_select_cb(Widget widget, XtPointer client_data,  
        XtPointer call_data)  
    {  
        char  
            *temp;  
        XmFileSelectionBoxCallbackStruct  
            *cbs = (XmFileSelectionBoxCallbackStruct *) call_data;  
        FSB_data_t  
            *BoxData = (FSB_data_t *) client_data;  
        XmStringGetLtoR(cbs->value, XmFONTLIST_DEFAULT_TAG, &temp);  
        strcpy(BoxData->filename, temp);  
        XtFree(temp);  
        XmStringGetLtoR(cbs->dir, XmFONTLIST_DEFAULT_TAG, &temp);  
        strcpy(BoxData->directory, temp);  
        XtFree(temp);  
        XmStringGetLtoR(cbs->pattern, XmFONTLIST_DEFAULT_TAG, &temp);  
        strcpy(BoxData->filter, temp);  
        XtFree(temp);  
        UF_UI_exit_dialog();  
    }  
    static void exit_cb(Widget widget, XtPointer client_data, XtPointer call_data)  
    {  
        FSB_data_t  
            *BoxData = (FSB_data_t *) client_data;  
        strcpy(BoxData->filename, "");  
        UF_UI_exit_dialog();  
    }  
    static void ask_filename(FSB_data_t *BoxData)  
    {  
        Arg  
            args[3];  
        Position  
            posX,  
            posY;  
        Widget  
            FileBox,  
            parent = UF_UI_get_default_parent();  
        UF_CALL(UF_UI_set_prompt(BoxData->cue));  
        UF_CALL(UF_UI_get_DA1_coords(&posX, &posY));  
        XtSetArg (args[0], XmNx, posX);  
        XtSetArg (args[1], XmNy, posY);  
        XtSetArg (args[2], XmNdefaultPosition, False);  
        FileBox = XmCreateFileSelectionDialog(parent, "FileBox", args, 3);  
        XtVaSetValues(XtParent(FileBox), XtVaTypedArg, XmNtitle, XmRString,  
            BoxData->title, strlen(BoxData->title)+1, NULL );  
        XtVaSetValues(FileBox, XtVaTypedArg, XmNpattern, XmRString,  
            BoxData->filter, strlen(BoxData->filter)+1, NULL );  
        XtVaSetValues(FileBox, XtVaTypedArg, XmNdirectory, XmRString,  
            BoxData->directory, strlen(BoxData->directory)+1, NULL );  
        XtAddCallback (FileBox, XmNcancelCallback, exit_cb, BoxData);  
        XtAddCallback (FileBox, XmNokCallback, file_select_cb, BoxData);  
        XtUnmanageChild(XmFileSelectionBoxGetChild(FileBox,XmDIALOG_HELP_BUTTON));  
        UF_UI_run_dialog(FileBox);  
    }  
    static void do_it(void)  
    {  
        FSB_data_t  
            BoxData = { "Choose text file to open",  
                        "Open Text File", "", "*.txt", "XXX" };  
        while (strcmp(BoxData.filename, ""))  
        {  
            ask_filename(&BoxData);  
            printf("The selected file was %s.\n", BoxData.filename);  
        }  
    }  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            do_it();  
            UF_CALL(UF_terminate());  
        }  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是NX的二次开发代码，其主要功能是创建一个文件选择对话框，允许用户选择一个文本文件，并获取用户选择的文件路径。
>
> 主要功能点包括：
>
> 1. 定义了一个FSB_data_t结构体，用于存储文件选择对话框的各种数据，包括提示信息、标题、初始目录、文件过滤器和文件名。
> 2. 定义了file_select_cb回调函数，当用户点击确定时，获取用户选择的文件路径等信息，并保存到FSB_data_t结构体中。
> 3. 定义了exit_cb回调函数，当用户点击取消时，清空文件名。
> 4. 定义了ask_filename函数，用于创建并显示文件选择对话框。
> 5. do_it函数中，循环调用ask_filename，直到用户没有选择文件时退出循环，并打印用户选择的文件路径。
> 6. ufusr函数是NX二次开发的标准入口函数，在这里初始化和终止NX Open，并调用do_it执行功能。
> 7. ufusr_ask_unload函数用于卸载动态库时立即返回。
>
> 总的来说，这段代码实现了文件选择对话框的功能，允许用户选择文件并获取文件路径，是NX二次开发中常用的一个功能。
>
