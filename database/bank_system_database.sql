--staff
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
      constraint pk_id primary key(id)
);

--default staff
insert into staff values (0, 'default', '000', 'default_address', '000', '10-10-2000', 'default_email', 'default_sector', '000', '2020-10-10 08:02:01', 0);


--support
create table support (
       id	     serial,
       staff_id      integer	  not null,
       opening_day   timestamp	  not null,	
       message	     varchar(501) not null,
       constraint pk_support_id primary key(id),
       constraint fk_staff_id foreign key(staff_id) references staff(id)
);

--blocked staff
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

--unlocked staff 
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

--blocked customer & account
create table blocked_customer_account (
       id			serial,
       responsible_staff_id	integer		not null,
       blocked_customer_id	integer       	not null,
       blocking_day		timestamp     	not null,
       reason			varchar(255) 	not null,
       constraint pk_blocked_customer_id primary key(id),
       constraint fk_responsible_staff_id foreign key(responsible_staff_id) references staff(id),
       constraint fk_blocked_customer_id foreign key(blocked_customer_id) references customer(id)
);

--count_customer_account_tiems
create table many_times_customer_account_blocked (
       id               serial,
       customer_id	integer		not null,
       times		integer		not null,
       constraint pk_count_blocked_customer_id primary key(customer_id),
       constraint fk_count_blocked_account_id foreign key(customer_id) references customer(id)
);

--unlock customer_account
create table unlocked_customer_account (
       id			serial,
       responsible_staff_id	integer		not null,
       unlocked_customer_id	integer       	not null,
       unlocking_day		timestamp     	not null,
       reason			varchar(255) 	not null,
       constraint pk_unlocked_customer_id primary key(id),
       constraint fk_responsible_staff_id foreign key(responsible_staff_id) references staff(id),
       constraint fk_unlocked_customer_account_id foreign key(unlocked_customer_id) references customer(id)
);

--change_customer_info
create table change_customer_info (
       id			serial,
       responsible_staff_id 	integer 	not null,
       changed_customer_id	integer		not null,
       changing_day 		timestamp 	not null,
       reason 			varchar(255) 	not null,
       constraint pk_change_customer_info_id primary key(id),
       constraint fk_responsible_staff_id foreign key(responsible_staff_id) references staff(id),
       constraint fk_changed_customer_id foreign key(changed_customer_id) references customer(id)
);

--previous_customer_information
create table previous_customer_information (
       id    			serial,
       change_table_id 		integer		not null,
       full_name    		varchar(100) 	not null,
       address      		varchar(50) 	not null,
       phone_number 		varchar(14) 	not null,
       email 	   		varchar(50) 	not null,
       password 	   	varchar(100) 	not null,
       constraint pk_previous_customer_info primary key(id),
       constraint fk_change_table_customer_id foreign key(change_table_id) references change_customer_info(id)
);

--deposit receipt
create table deposit_receipt (
       id		    serial,
       customer_id          integer      not null,
       account_id           integer      not null,
       amount 	  	    real         not null,
       who_did_name         varchar(100) not null,
       who_did_cpf          varchar(11)  not null,
       responsible_staff_id integer      not null,
       deposit_day          timestamp 	 not null,
       deposit_receipt_key  varchar(100) not null,
       constraint pk_deposit_id primary key(id),
       constraint fk_acc_id foreign key(account_id) references account(id),
       constraint fk_responsible_staff foreign key(responsible_staff_id) references staff(id)
);

--withdrawal receipt
create table withdrawal_receipt (
       id		    serial,
       customer_id          integer      not null,
       account_id           integer      not null,
       amount 	  	    real         not null,
       responsible_staff_id integer      not null,
       withdrawal_day       timestamp 	 not null,
       deposit_receipt_key  varchar(100) not null,
       constraint pk_withdrawal_id primary key(id),
       constraint fk_acc_withdrawal_id foreign key(account_id) references account(id),
       constraint fk_responsible_staff_id foreign key(responsible_staff_id) references staff(id)
);

--transfer voucher
create table transfer_voucher (
       id		     serial,
       from_customer_id      integer      not null,
       from_account_id       integer      not null,
       amount 	  	     real         not null,
       to_customer_id        integer      not null,
       to_account_id         integer      not null,
       responsible_staff_id  integer      not null,
       transfer_day          timestamp 	  not null,
       transfer_voucher_key  varchar(100) not null,
       constraint pk_transfer_id primary key(id),
       constraint fk_from_customer_id foreign key(from_customer_id) references customer(id),
       constraint fk_from_acc__id foreign key(from_account_id) references account(id),
       constraint fk_to_customer_id foreign key(to_customer_id) references customer(id),
       constraint fk_to_acc__id foreign key(to_account_id) references account(id),
       constraint fk_responsible_staff_id foreign key(responsible_staff_id) references staff(id)
);

--loan receipt
create table loan_receipt (
       id                    serial,
       customer_id	     integer      not null,
       account_id            integer 	  not null,
       amount 	  	     real         not null,
       payment_installments  integer      not null,
       fees                  real         not null,
       amount_per_month      real         not null,
       payday		     integer      not null,
       loan_date             timestamp    not null,
       responsible_staff_id  integer      not null,
       loan_receipt_key      varchar(100) not null,
       constraint pk_loan_key primary key(loan_receipt_key),
       constraint fk_customer foreign key(customer_id) references customer(id),
       constraint fk_account foreign key(account_id) references account(id),       
       constraint fk_responsible_staff foreign key(responsible_staff_id) references staff(id)
);

--loan payment control
create table loan_payment_control (
       id                      serial,
       payer_name                varchar(100) not null,
       payer_cpf                 varchar(11)  not null,
       loan_receipt_key          varchar(100) not null,
       payment_installments_paid integer      not null,
       payment_day 	         timestamp    not null,
       on_time 		         varchar(3)   not null,
       constraint pk_loan_control_id primary key(id),
       constraint fk_loan_receipt foreign key(loan_receipt_key) references loan_receipt(loan_receipt_key)
);
