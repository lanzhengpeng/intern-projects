### 【2606】select a sketch 选择一个草图

#### 代码

```cpp
    static tag_t select_a_sketch(char *prompt)  
    {  
        int  
            resp;  
        tag_t  
            sketch;  
        if (!UF_CALL(UF_UI_select_sketch(prompt, NULL, &sketch, &resp))  
        && (resp == UF_UI_OK))  
           return sketch;  
        else  
            return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是NX二次开发代码，用于选择一个草图。以下是代码的主要功能和实现步骤：
>
> 1. 函数定义：select_a_sketch是一个静态函数，用于选择一个草图，并返回所选草图的tag。
> 2. 参数：函数接收一个字符串参数prompt，作为选择草图的提示信息。
> 3. 选择草图：使用NX提供的UF_UI_select_sketch函数，在当前活动部件中弹出一个选择草图对话框，并根据提示信息选择草图。
> 4. 返回值：如果选择成功，则返回所选草图的tag；如果选择失败，则返回NULL_TAG。
> 5. 错误处理：使用UF_CALL宏调用NX函数，如果调用失败，则函数返回NULL_TAG。
> 6. 提示信息：prompt参数用于在对话框中显示提示信息，引导用户选择正确的草图。
> 7. 选择结果处理：函数的返回值用于在后续代码中引用所选草图，进行进一步的操作。
> 8. 函数封装：使用静态函数封装选择草图的逻辑，便于在其他函数中调用，提高代码复用性。
>
> 总体来说，这段代码实现了在NX中弹出草图选择对话框，并根据用户选择返回草图tag的功能，为后续草图操作提供了基础。
>
