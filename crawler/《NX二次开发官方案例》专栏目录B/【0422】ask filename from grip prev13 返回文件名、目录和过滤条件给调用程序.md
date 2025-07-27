### 【0422】ask filename from grip prev13 返回文件名、目录和过滤条件给调用程序

#### 代码

```cpp
    /*HEAD ASK_FILENAME_FROM_GRIP_PREV13 CCC UFUN */  
    /*  If you are running V13.0 or higher, see ask_filename_from_grip.c where a  
        new function UF_UI_create_filebox.  
        This is a generic UG/Open API (Ufunc) program which can be called whenever  
        interactive file selection is required in a GRIP program.  It will put up  
        a file selection dialog.  You can specify the "Cue", dialog title, initial  
        directory, and filter.  The selected filename is returned to the GRIP  
        program.  If the user selects Cancel the filename will come back &nulstr.  
        Since the directory and filter can be changed by the user, they are also  
        returned to the calling GRIP program, so if necessary the program can be  
        called again and the dialog will start where it was left off.  
        Here is sample GRIP code demonstating how to use the program:  
            string/ cue(80), title(80), directory(132), filter(30), filename(132)  
            grargs/ cue, title, directory, filter, filename  
            cue = 'Choose text file to open'  
            title = 'Open Text File'  
            directory = &nulstr   $$ &nulstr will default to current directory  
            filter = '*.txt'  
    a10:    XSPAWN/ ufun, '/path/to/internal/program/grip_ask_filename'  
            ifthen/ filename <> &nulstr  
                fetch/ txt, 2, filename, iferr, x10:  
                fprint/ 2  
            endif  
            halt  
    x10:    messg/'Cannot open text file', 'Please select another'  
            jump/ a10:  
    */  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文中的内容，翻译如下：

注意：UF_print_syslog在V18版本中是新增的。 */  
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
    static void file_select_cb(Widget widget,  
        XtPointer client_data, XtPointer call_data)  
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
            BoxData;  
        UF_args_t  
            grip_args[5];  
        grip_args[0].type = UF_TYPE_CHAR;  
        grip_args[0].length = 0;  
        grip_args[0].address = BoxData.cue;  
        grip_args[1].type = UF_TYPE_CHAR;  
        grip_args[1].length = 0;  
        grip_args[1].address = BoxData.title;  
        grip_args[2].type = UF_TYPE_CHAR;  
        grip_args[2].length = 0;  
        grip_args[2].address = BoxData.directory;  
        grip_args[3].type = UF_TYPE_CHAR;  
        grip_args[3].length = 0;  
        grip_args[3].address = BoxData.filter;  
        grip_args[4].type = UF_TYPE_CHAR;  
        grip_args[4].length = 0;  
        grip_args[4].address = BoxData.filename;  
        UF_CALL(UF_ask_grip_args(5, grip_args));  
        ask_filename(&BoxData);  
        UF_CALL(UF_set_grip_args(5, grip_args));  
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

> 这段代码是用于在NX中实现文件选择对话框的UFUNC函数。其基本功能包括以下几点：
>
> 1. 定义了一个FSB_data_t结构体，用于存储对话框的各种参数，如提示信息、标题、初始目录、文件过滤器和文件名。
> 2. 实现了report_error函数，用于打印UF函数的错误信息。
> 3. 实现了file_select_cb函数，作为文件选择对话框的确认回调函数，用于获取用户选择的文件名、目录和过滤器，并将其存储到FSB_data_t结构体中。
> 4. 实现了exit_cb函数，作为文件选择对话框的取消回调函数，用于清空文件名。
> 5. 实现了ask_filename函数，用于创建文件选择对话框，并设置各种参数。
> 6. do_it函数用于获取GRIP传入的参数，调用ask_filename函数显示对话框，并将用户选择的文件名传回给GRIP。
> 7. ufusr函数是UFUNC的主函数，用于初始化和结束NX API，并调用do_it函数。
> 8. ufusr_ask_unload函数用于卸载UFUNC。
>
> 总体来说，这段代码实现了在NX中通过GRIP参数传入提示信息、标题、初始目录和文件过滤器，弹出一个文件选择对话框，让用户选择文件，并将选择的文件名传回给GRIP的功能。
>
