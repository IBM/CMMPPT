COPY printer(name, location, "prodRate", "F175", "F285", "PETG", "PLA", "ABS", "ONYX") TO '/home/ubuntu/git/cmmppt/visor/data/ThompkinsShare/printer.sql.csv' 
WITH (FORMAT CSV, HEADER, QUOTE '"', FORCE_QUOTE(name,location,"F175", "F285", "PETG", "PLA", "ABS", "ONYX"));
COPY requestQuantity TO '/home/ubuntu/git/cmmppt/visor/data/ThompkinsShare/requestQuantity.sql.csv' WITH (FORMAT CSV, HEADER, QUOTE '"', FORCE_QUOTE(location));
COPY onHandMaterial TO '/home/ubuntu/git/cmmppt/visor/data/ThompkinsShare/onHandMaterial.sql.csv' WITH (FORMAT CSV, HEADER, QUOTE '"', FORCE_QUOTE(location, "filamentSize", "plasticType", "quantity", "share"));
