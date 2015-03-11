# MSE2202-Project-2015
Here it is, the Github for the dream team 

Ok team I have very high hopes for this project but there are some key things that will make the overall experience better IMO. 
These are just the thoughts I have so please dont be discouraged if you think you have a better approach :) #goTeam
also note i sometimes say pull request when it might actually be a push (but youll know what i mean)

1) Github
I've spent some time trying to figure out how to effectively use Github and im gonna talk to Naish Monday but heres how Im understanding this will work best (because there multiple ways you can organize projects and collabourations)
  
  I created an organiztion and will add you all as owners (this gives everyone FULL rights/administrative access to everything in the organition, just be careful if you need to delete..). I also created a repository for our project. Currnently it only has two branches: Master (this is for 100% debugged and working code) and Dev (essentially the in progress code). I think it might also be best to create an individual branch for each of us. This would allow us to work up multiple parts simulaniously (one could work on door opening while another person works on locating waterbottle etc). It would also mean one of use working on our own does not need to worry about ruining working code by adding new steps (their personal branch can be tested before pushing to Dev). an alternate approach could be making branches for different steps in the course (ie door stage, newspaper stage) (This might be better and I have a thought that's way easier to explain with a diagram) 

  From here I'm not quite sure which of two opions would be best to take since we all have full access. 
    a) you will all need to clone the repository to your computer (this is so you can do work on your own computer offline).
  I believe, because we all have full access, that after this you can edit/commmit/pull/push/sync any branch on your own       and it will change the main repository.
    b) we may however have to fork the main repository (from the organization) to our own Github account, then clone the       forked repository (in your personal account) onto your computer. If done this way you can edit/commit/pull/push/sync with    your personal repository, but then must create a pull request in order to make these changes on the main repository. 
  see this link if you have trouble understaning b)                         
  https://support.rightscale.com/12-Guides/Chef_Cookbooks_Developer_Guide/04-Developer/Source_Control_Management_Systems/GitHub/Clone_vs._Fork
  
Once everyone has a local repository (term used for the file saved on your computer) we can edit/commit to our indervidual branches, then after testing we can submit pull/push requests to merge branches.

Github conventions/my suggestions:
i) NEVER NEVER NEVER write new code or edit in master branch (this branch is for finished code and should only be changed by pushing to it). I would also highly suggest that pushing to master (and posibly dev) be a group decision.
ii) I highly suggest not writing new sections of code in dev branch as well, IMO I think this branch should be used for debugging/editing the code if it does not work after being pushed to from another branch. (ex. Ev writes code in her branch that succesully opens door, Cam writes code in his branch that succesfully grabs the bottle. both push their code to dev but then they dont work together, so we debug it in dev until it works then push to master)
iii) please make sure we have selected the correct branch on the Github computer application so that we do not accidently edit the wrong branch.


2) Code writing conventions
Coding is tough, coding in a group is tougher. we all need to be on the same page before we start writing code so that we can write more efficiently, and so we can understand eachother code. we will have to discus this as a group but here are some things I thought off (again nothing set in stone)

  a) COMMENT COMMENT COMMENT Quazi was right kids, commenting takes like 2 seconds when you know what your code does, and makes it infinitly times easier for others to understand your code.
  b) we need to establish a convention for variable/functions/etc. heres kinda how i like to write (influenced by cam)

  variable: int stopCounters; (first letter of first world lower case (lc), any following words begin with uc)
  variable with unique type (const, unsigned, ect): const int ci_NumberOfStops, servo_RightMotor;
  functions: void ReadLineTrackers(); void Ping(); (all first letters capatilized)
  stucts: if we even use them id probly name them similar to functions
  
  c) please chose logical variables, not overly long, abriviate when neccessary but don't OvrAbrEvryWrd
  i usually like to think, could someone whos never seen this code guess what this variable is for? if(yes){cout<<"Winner"};
  
Thats pretty much all i can think off, any suggestions are welcome 













