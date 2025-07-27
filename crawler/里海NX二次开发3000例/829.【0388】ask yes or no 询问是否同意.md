### 【0388】ask yes or no 询问是否同意

#### 代码

```cpp
    static logical ask_yes_or_no(char *title, char *message)  
    {  
        int  
            resp;  
        UF_UI_message_buttons_t  
            buttons = { TRUE, FALSE, TRUE, "Yes", NULL, "No", 1, 0, 2 };  
        UF_CALL(UF_UI_message_dialog(title, UF_UI_MESSAGE_QUESTION, &message, 1,  
            TRUE, &buttons, &resp));  
        if (resp == 1) return TRUE;  
        else return FALSE;  
    }

```

#### 代码解析

> 这段代码是NX二次开发中用于弹出一个询问框，让用户选择是或否的函数ask_yes_or_no。
>
> 函数的基本流程如下：
>
> 1. 定义一个结构体buttons，用于设置对话框的按钮选项，包括显示是按钮、否按钮、显示帮助按钮、是按钮文本、否按钮文本、默认按钮等。
> 2. 调用UF_UI_message_dialog函数，弹出对话框。对话框的标题为title，类型为询问框，显示的消息为message，按钮选项为buttons。
> 3. UF_UI_message_dialog函数调用完成后，会返回用户的选择结果到变量resp中。
> 4. 根据resp的值判断用户的选择，如果为1则表示用户选择是，函数返回TRUE。否则，表示用户选择否，函数返回FALSE。
> 5. 这个函数可以用于在程序中需要让用户确认操作的场景。
>
> 总的来说，这段代码实现了在NX中弹出询问框的功能，用户可以选择是或否来确认或取消操作。
>
