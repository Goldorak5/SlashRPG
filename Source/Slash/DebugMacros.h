#pragma once
#include "DrawDebugHelpers.h"



#define DRAW_SPHERE(Location) if (GetWorld()) DrawDebugSphere(GetWorld(),Location, 25.f, 12, FColor::Red,true);
#define DRAW_SPHERE_SF(Location) if (GetWorld()) DrawDebugSphere(GetWorld(),Location, 25.f, 20, FColor::Red,false, (-1.0F));
#define DRAW_SPHERE_SMALL(Location,Color) if (GetWorld()) DrawDebugSphere(GetWorld(),Location, 10.f, 12, Color, false, 2.f);


#define DRAW_LINE(StartLocation,EndLocation) if(GetWorld()) DrawDebugLine(GetWorld(),Location, EndLocation,FColor::Red,true, -1.f, 0, 1.f);
#define DRAW_LINE_SF(StartLocation,EndLocation) if(GetWorld()) DrawDebugLine(GetWorld(),Location, EndLocation,FColor::Red,false, -1.f, 0, 1.f);

#define DRAW_POINT(Location) if(GetWorld()) DrawDebugPoint(GetWorld(), Location, 15.f, FColor::Red, true);
#define DRAW_POINT_SF(Location) if(GetWorld()) DrawDebugPoint(GetWorld(), Location, 15.f, FColor::Red, false,-1.f);


#define DRAW_VECTOR(StartLocation, EndLocation) if(GetWorld()) \
{\
DrawDebugLine(GetWorld(),StartLocation, EndLocation,FColor::Red,true, -1.f, 0, 1.f);\
DrawDebugPoint(GetWorld(), EndLocation, 15.f, FColor::Red, true);\
 }
#define DRAW_VECTOR_SF(StartLocation, EndLocation) if(GetWorld()) \
{\
DrawDebugLine(GetWorld(),StartLocation, EndLocation,FColor::Red,false, -1.f, 0, 1.f);\
DrawDebugPoint(GetWorld(), EndLocation, 15.f, FColor::Red, false,-1.f);\
 }

#define DRAW_ARROW(StartLocation,EndLocation) if(GetWorld()) DrawDebugDirectionalArrow(GetWorld(),Location, EndLocation,20.f, FColor::Green,true);