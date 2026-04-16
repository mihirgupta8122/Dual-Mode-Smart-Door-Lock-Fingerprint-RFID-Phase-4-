
#pragma once
class AccessMethod{ public: virtual ~AccessMethod()=default; virtual bool tryAuthenticate()=0; };
