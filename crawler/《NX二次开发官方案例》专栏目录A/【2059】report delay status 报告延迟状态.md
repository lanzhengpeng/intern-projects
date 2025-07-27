### 【2059】report delay status 报告延迟状态

#### 代码

```cpp
    static void report_delay_status( UF_WAVE_delay_status_t delay_status )  
    {  
        switch ( delay_status )  
        {  
            case UF_WAVE_not_delayed:  
            WRITE( "The part is not delayed.\n" );  
            break;  
            case UF_WAVE_session_delayed:  
            WRITE( "The part is delayed because the session is delayed.\n" );  
            break;  
            case UF_WAVE_partial_load_delayed:  
            WRITE( " The part is delayed because it's partially loaded.\n" );  
            break;  
            case UF_WAVE_session_frozen:  
            WRITE( "The part is frozen in this session.\n" );  
            break;  
            case UF_WAVE_persistent_frozen:  
            WRITE( "The part is frozen for all sessions.\n" );  
            break;  
            case UF_WAVE_not_in_assembly_delayed:  
            WRITE( "Part delayed: it is not in the displayed assembly.\n" );  
            break;  
        }  
    }

```

#### 代码解析

> 这段代码是用于报告NX模型中部件延迟状态的二次开发代码。其中定义了一个静态函数 report_delay_status，用于接收一个延迟状态枚举值，并根据该值输出不同的提示信息。延迟状态枚举包括以下几种情况：
>
> 1. UF_WAVE_not_delayed: 表示部件没有延迟。
> 2. UF_WAVE_session_delayed: 表示部件由于会话延迟而延迟。
> 3. UF_WAVE_partial_load_delayed: 表示部件由于部分加载而延迟。
> 4. UF_WAVE_session_frozen: 表示部件在当前会话中冻结。
> 5. UF_WAVE_persistent_frozen: 表示部件对所有会话冻结。
> 6. UF_WAVE_not_in_assembly_delayed: 表示部件由于不在当前显示的装配体中而延迟。
>
> 通过调用这个函数并传入不同的枚举值，可以获取模型中各个部件的延迟状态，并输出相应的提示信息，从而方便用户了解当前模型中各个部件的状态。
>
