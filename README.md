# Machine Identity Demo

## Directories
### assets
The directory in which the references to data assets are stored. These will be shared with the consumer of data.
### database
The directory simulating the database in which data is stored to be shared.
### source
The diretory simulating the source of data which could be a sensor on the Raspberry.

## Steps
### Step 1 - Create machine identity

´´´create_mi.py .... ´´´

### Step 2 - Read data from machine, store it and save the asset on R3C

´´´produce_data.py machine.identity source/input_data_1.json ´´´

Output:
database/data_1.json (which is simply the input plus maybe the identity of machine generating the data?)
assets/assets_1.json (the reference to the R3C entry)

### Step 3 - Get asset identifier from R3C, retrieve data and verify its integrity and source

´´´consume_data.py assets/asset_1.json´´´

Output
- data_1.json - just dump it on the screen
- mesage telling the data is not tampered
- information on the machine - source/signer of data
