//
//  export.h
//  BehaviorAnalysis
//
//  Created by luolijun on 2020/2/19.
//  Copyright © 2020 baidu. All rights reserved.
//

#ifndef exports_h
#define exports_h

#ifndef EXPORT_API
#if defined(__cplusplus)
#define EXPORT_API extern "C"
#else
#define EXPORT_API extern
#endif
#endif

#endif /* exports_h */
