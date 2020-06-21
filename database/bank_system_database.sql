--staff table
create table staff (
      id	   serial, 
      full_name    varchar(100) not null,
      cpf          varchar(11) 	not null,
      address      varchar(50) 	not null,
      phone_number varchar(14) 	not null,
      birthday     date		not null,
      email 	   varchar(50) 	not null,
      sector 	   varchar(10) 	not null,
      password 	   varchar(100)	not null,
      created_in   timestamp 	not null,
      by_staff	   integer	not null,
      constraint pk_id primary key(id),
);

--support table
create table support (
       id	     serial,
       staff_id      integer	  not null,
       opening_day   timestamp	  not null,	
       message	     varchar(501) not null,
       constraint pk_support_id primary key(id),
       constraint fk_staff_id foreign key(staff_id) references staff(id)
);

--blocked staff table
create table blocked_staff (
       id			serial,
       responsible_staff_id	integer		not null,
       blocked_staff_id		integer       	not null,
       blocking_day		timestamp     	not null,
       reason			varchar(255) 	not null,
       constraint pk_blocked_id primary key(id),
       constraint fk_responsible_staff_id foreign key(responsible_staff_id) references staff(id),
       constraint fk_blocked_staff_id foreign key(blocked_staff_id) references staff(id)
);

--unlocked staff table
create table unlocked_staff (
       id			serial,
       responsible_staff_id	integer		not null,
       unlocked_staff_id	integer       	not null,
       unlocking_day		timestamp     	not null,
       reason			varchar(255) 	not null,
       constraint pk_unlocked_id primary key(id),
       constraint fk_responsible_staff_id foreign key(responsible_staff_id) references staff(id),
       constraint fk_unlocked_staff_id foreign key(unlocked_staff_id) references staff(id)
);

--count_blocked_staff
create table many_times_staff_blocked (
       id               serial,
       staff_id		integer		not null,
       times		integer		not null,
       constraint pk_count_blocked_staff_id primary key(staff_id),
       constraint fk_count_blocked_staff_id foreign key(staff_id) references staff(id)
);

--change_staff_info
create table change_staff_info (
       id			serial,
       responsible_staff_id 	integer 	not null,
       changed_staff_id		integer		not null,
       changing_day 		timestamp 	not null,
       reason 			varchar(255) 	not null,
       constraint pk_change_info_id primary key(id),
       constraint fk_responsible_staff_id foreign key(responsible_staff_id) references staff(id),
       constraint fk_changed_staff_id foreign key(changed_staff_id) references staff(id)
);

--previous_staff_information
create table previous_staff_information (
       id    			serial,
       change_table_id 		integer		not null,
       full_name    		varchar(100) 	not null,
       address      		varchar(50) 	not null,
       phone_number 		varchar(14) 	not null,
       email 	   		varchar(50) 	not null,
       sector 	   		varchar(10) 	not null,
       password 	   	varchar(100) 	not null,
       constraint pk_previous_staff_info primary key(id),
       constraint fk_change_table_id foreign key(change_table_id) references change_staff_info(id)
);

--customer
create table customer (
       id			serial,
       full_name		varchar(100)	not null,
       cpf            		varchar(11) 	not null,
       address        		varchar(50) 	not null,
       phone_number   		varchar(14) 	not null,
       birthday       		date		not null,
       email 	      		varchar(50) 	not null,
       created_in     		timestamp 	not null,
       responsible_staff_id	integer		not null,
       constraint pk_customer_id primary key(id),
       constraint fk_customer_responsible_staff foreign key(responsible_staff_id) references staff(id)
);

--account
create table account (
       id    	      serial,
       account_number serial,	
       customer_id    integer	     not null,
       amount         real           not null,
       password       varchar(100)   not null,
       constraint pk_account_id primary key(id),
       constraint fk_customer_id foreign key(customer_id) references customer(id)
);

--opening receipt
create table opening_receipt (
      id	      serial,
      account_number  integer      not null,
      initial_amount  real         not null,
      receipt_key     varchar(100) not null,
      constraint pk_opening_receipt_accout primary key(account_number),
      constraint fk_opening_receipt_account_number foreign key(account_number) references account(id)
);
