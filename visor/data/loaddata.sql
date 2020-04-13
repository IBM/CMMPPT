 -- DELETE FROM printer;
-- COPY printer(name, location, "prodRate", "F175", "F285", "PETG", "PLA", "ABS", "ONYX") FROM  '/home/djensen/git/CMMPPT/visor/data/ThompkinsShare/printer.csv' DELIMITER ',' CSV HEADER;
  COPY printer(name, location, "prodRate", "F175", "F285", "PETG", "PLA", "ABS", "ONYX") FROM  STDIN DELIMITER ',' CSV HEADER;
--  DELETE FROM onHandMaterial;
--  COPY onHandMaterial(location, "filamentSize", "plasticType", "quantity", "share") FROM  '/home/ubuntu/git/cmmppt/visor/data/ThompkinsShare/onHandMaterial.csv' DELIMITER ',' CSV HEADER;
-- DELETE FROM requestQuantity;
-- COPY requestQuantity(location, "date", "requestedQuantity") FROM  '/home/ubuntu/git/cmmppt/visor/data/ThompkinsShare/requestQuantity.csv' DELIMITER ',' CSV HEADER;
