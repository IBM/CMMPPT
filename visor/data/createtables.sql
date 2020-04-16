DROP TABLE onhandmaterial;
DROP TABLE printer;
DROP TABLE requestQuantity;
DROP TABLE shipQuantity;
DROP TABLE subVol;
DROP TABLE prodVol;
create table printer (
    "name"  VARCHAR, 
    "location" VARCHAR,
    "prodRate" FLOAT,
    "F175" BOOLEAN,
    "F285" BOOLEAN,
    "PETG" BOOLEAN,
    "PLA" BOOLEAN,
    "ABS" BOOLEAN,
    "ONYX" BOOLEAN,
    PRIMARY KEY("name", "location")
);

create table onHandMaterial (
    "location" VARCHAR,
    "filamentSize" VARCHAR CHECK("filamentSize" = '1.75mm' or "filamentSize" = '2.85mm'),
    "plasticType" VARCHAR CHECK("plasticType" = 'PETG' or "plasticType" = 'PLA' or "plasticType" = 'ABS' or "plasticType" = 'ONYX'),
    "quantity" FLOAT,
    "share" INTEGER CHECK( share >=0 and share <= 100),
    PRIMARY KEY("location", "filamentSize","plasticType" )
);
create table requestQuantity (
    "location" VARCHAR,
    "date" INTEGER CHECK (date >=0 ),
    "requestedQuantity" FLOAT,
    PRIMARY KEY("location", "date" )
);
create table shipQuantity (
    "requestingLocation" VARCHAR,
    "producingLocation" VARCHAR,
    "period" INTEGER,
    "shipQuantity" FLOAT,
    PRIMARY KEY("requestingLocation", "producingLocation", "period" )
);
create table subVol (
    "printerName" VARCHAR,
    "printerLocation" VARCHAR,
    "materialLocation" VARCHAR,
    "filamentSize" VARCHAR CHECK("filamentSize" = '1.75mm' or "filamentSize" = '2.85mm'),
    "plasticType" VARCHAR CHECK("plasticType" = 'PETG' or "plasticType" = 'PLA' or "plasticType" = 'ABS' or "plasticType" = 'ONYX'),    
    "period" INTEGER,
    "quantity" FLOAT,
    "own"  BOOLEAN,
    PRIMARY KEY("printerName", "printerLocation", "materialLocation", "filamentSize",  "plasticType", "period", "own")
);
create table prodVol (
    "printerName" VARCHAR,
    "printerLocation" VARCHAR,
    "materialLocation" VARCHAR,
    "period" INTEGER,
    "quantity" FLOAT,
    "own"  BOOLEAN,
    PRIMARY KEY("printerName", "printerLocation", "period")
);

-- INSERT INTO onHandMaterial (location, "filamentSize", "plasticType", quantity, share) VALUES ('a','1.75mm', 'PETG', 0.5, 38);