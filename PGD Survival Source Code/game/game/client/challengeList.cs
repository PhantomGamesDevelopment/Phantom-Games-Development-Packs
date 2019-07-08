//challengeList.cs
//Phantom139
//Handles the challenge list

//Little script hack kiddies shouldn't be looking here, you're just wasting your time ;)

//Add the weapons to the game's challenge system
addWeapon("Ryder");
addWeapon("Duke");
addWeapon("Lurker");
addWeapon("Disposition");
addWeapon("Swarmer");
addWeapon("Rocket Launcher");
addWeapon("Mamba");
addWeapon("Kral");
addWeapon("Kraad");
addWeapon("Kralmok");
addWeapon("Tesla Forcer");

//Add the Challenge Trees
addChallengeTree("Ryder");
addChallengeTree("Duke");
addChallengeTree("Lurker");
addChallengeTree("Disposition");
addChallengeTree("Swarmer");
addChallengeTree("Rocket Launcher");
addChallengeTree("Mamba");
addChallengeTree("Kral");
addChallengeTree("Kraad");
addChallengeTree("Kralmok");
addChallengeTree("Tesla Forcer");

//Add the challenges
//Ryder Pistol Challenges
addChallenge(0, "Ryder Marksman I", "Get 5 Kills using the Ryder Pistol", "0/5", "25EXP");
addChallenge(0, "Ryder Marksman II", "Get 15 Kills using the Ryder Pistol", "0/15", "50EXP");
addChallenge(0, "Ryder Marksman III", "Get 50 Kills using the Ryder Pistol", "0/50", "100EXP");
addChallenge(0, "Ryder Marksman IV", "Get 150 Kills using the Ryder Pistol", "0/150", "250EXP");
addChallenge(0, "Ryder Marksman V", "Get 250 Kills using the Ryder Pistol", "0/250", "500EXP");
addChallenge(0, "Ryder Marksman VI", "Get 500 Kills using the Ryder Pistol", "0/500", "1000EXP");
addChallenge(0, "Ryder Marksman VII", "Get 1000 Kills using the Ryder Pistol", "0/1000", "2500EXP");
addChallenge(0, "Ryder Marksman VIII", "Get 2500 Kills using the Ryder Pistol", "0/2500", "5000EXP");
addChallenge(0, "Ryder Marksman IX", "Get 5000 Kills using the Ryder Pistol", "0/5000", "10000EXP");

addChallenge(0, "Ryder Sharpshooter I", "Get 10 Headshot Kills with the Ryder Pistol", "0/10", "50EXP");
addChallenge(0, "Ryder Sharpshooter II", "Get 50 Headshot Kills with the Ryder Pistol", "0/50", "100EXP");
addChallenge(0, "Ryder Sharpshooter III", "Get 100 Headshot Kills with the Ryder Pistol", "0/100", "250EXP");
addChallenge(0, "Ryder Sharpshooter IV", "Get 250 Headshot Kills with the Ryder Pistol", "0/250", "500EXP");
addChallenge(0, "Ryder Sharpshooter V", "Get 750 Headshot Kills with the Ryder Pistol", "0/750", "1000EXP");
addChallenge(0, "Ryder Sharpshooter VI", "Get 1500 Headshot Kills with the Ryder Pistol", "0/1500", "2000EXP");

addChallenge(0, "Torcher", "Incinerate 75 Zombies with the Ryder Incendinary Rounds", "0/75", "500EXP");
addChallenge(0, "Nothin' But Dust!", "Disintegrate 150 Zombies with the Ryder Pulse Rounds", "0/150", "500EXP");
addChallenge(0, "Help in Small Packages", "Heal 5000 Points of Friendly Health using the Medic Specialization.", "0/5000", "500EXP");

addChallenge(0, "Ryder Apprentice", "Reach Affinity Level 15 for this Weapon", "1/15", "250EXP");
addChallenge(0, "Ryder Expert", "Reach Affinity Level 25 for this Weapon", "1/25", "500EXP");
addChallenge(0, "Ryder Mastery", "Reach Affinity Level 35 for this Weapon", "1/35", "750EXP");
addChallenge(0, "Ryder Overlord", "Reach the maximum Affinity Level for this Weapon", "1/50", "0EXP");

//Duke Pistol Challenges
addChallenge(1, "Duke Marksman I", "Get 5 Kills using the Duke Pistol", "0/5", "25EXP");
addChallenge(1, "Duke Marksman II", "Get 15 Kills using the Duke Pistol", "0/15", "50EXP");
addChallenge(1, "Duke Marksman III", "Get 50 Kills using the Duke Pistol", "0/50", "100EXP");
addChallenge(1, "Duke Marksman IV", "Get 150 Kills using the Duke Pistol", "0/150", "250EXP");
addChallenge(1, "Duke Marksman V", "Get 250 Kills using the Duke Pistol", "0/250", "500EXP");
addChallenge(1, "Duke Marksman VI", "Get 500 Kills using the Duke Pistol", "0/500", "750EXP");
addChallenge(1, "Duke Marksman VII", "Get 1000 Kills using the Duke Pistol", "0/1000", "1500EXP");

addChallenge(1, "Duke Sharpshooter I", "Get 10 Headshot Kills with the Duke Pistol", "0/10", "50EXP");
addChallenge(1, "Duke Sharpshooter II", "Get 50 Headshot Kills with the Duke Pistol", "0/50", "100EXP");
addChallenge(1, "Duke Sharpshooter III", "Get 100 Headshot Kills with the Duke Pistol", "0/100", "250EXP");
addChallenge(1, "Duke Sharpshooter IV", "Get 250 Headshot Kills with the Duke Pistol", "0/250", "500EXP");
addChallenge(1, "Duke Sharpshooter V", "Get 750 Headshot Kills with the Duke Pistol", "0/750", "1000EXP");
addChallenge(1, "Duke Sharpshooter VI", "Get 1500 Headshot Kills with the Duke Pistol", "0/1500", "2000EXP");

addChallenge(1, "Blaster I", "Get 25 One Shot Kills with the Duke Pistol", "0/25", "150EXP");
addChallenge(1, "Blaster II", "Get 75 One Shot Kills with the Duke Pistol", "0/75", "350EXP");
addChallenge(1, "Blaster III", "Get 200 One Shot Kills with the Duke Pistol", "0/200", "500EXP");
addChallenge(1, "Blaster IV", "Get 500 One Shot Kills with the Duke Pistol", "0/500", "1000EXP");
addChallenge(1, "Blaster V", "Get 1000 One Shot Kills with the Duke Pistol", "0/1000", "1500EXP");

addChallenge(1, "Explosions!!!!", "Kill 150 Zombies with the Shockwave Specialization", "0/150", "500EXP");
addChallenge(1, "Gone IN The Wind", "Disintegrate 150 Zombies with the Pulse Rounds Specialization", "0/150", "750EXP");

addChallenge(1, "Duke Apprentice", "Reach Affinity Level 15 for this Weapon", "1/15", "250EXP");
addChallenge(1, "Duke Expert", "Reach Affinity Level 25 for this Weapon", "1/25", "500EXP");
addChallenge(1, "Duke Mastery", "Reach Affinity Level 35 for this Weapon", "1/35", "750EXP");
addChallenge(1, "Duke Overlord", "Reach the maximum Affinity Level for this Weapon", "1/50", "0EXP");

//Lurker Challenges
addChallenge(2, "Lurker Marksman I", "Get 15 Kills using the Lurker", "0/15", "25EXP");
addChallenge(2, "Lurker Marksman II", "Get 50 Kills using the Lurker", "0/50", "50EXP");
addChallenge(2, "Lurker Marksman III", "Get 150 Kills using the Lurker", "0/150", "100EXP");
addChallenge(2, "Lurker Marksman IV", "Get 300 Kills using the Lurker", "0/300", "250EXP");
addChallenge(2, "Lurker Marksman V", "Get 750 Kills using the Lurker", "0/750", "500EXP");
addChallenge(2, "Lurker Marksman VI", "Get 1500 Kills using the Lurker", "0/1500", "750EXP");
addChallenge(2, "Lurker Marksman VII", "Get 3000 Kills using the Lurker", "0/3000", "1500EXP");

addChallenge(2, "Lurker Sharpshooter I", "Get 15 Headshot Kills with the Lurker", "0/15", "50EXP");
addChallenge(2, "Lurker Sharpshooter II", "Get 50 Headshot Kills with the Lurker", "0/50", "100EXP");
addChallenge(2, "Lurker Sharpshooter III", "Get 100 Headshot Kills with the Lurker", "0/100", "250EXP");
addChallenge(2, "Lurker Sharpshooter IV", "Get 250 Headshot Kills with the Lurker", "0/250", "500EXP");
addChallenge(2, "Lurker Sharpshooter V", "Get 750 Headshot Kills with the Lurker", "0/750", "1000EXP");
addChallenge(2, "Lurker Sharpshooter VI", "Get 1500 Headshot Kills with the Lurker", "0/1500", "2000EXP");

addChallenge(2, "Incinerator I", "Incinerate 25 Zombies with the Incendinary Rounds Specialization", "0/25", "50EXP");
addChallenge(2, "Incinerator II", "Incinerate 100 Zombies with the Incendinary Rounds Specialization", "0/100", "100EXP");
addChallenge(2, "Incinerator III", "Incinerate 250 Zombies with the Incendinary Rounds Specialization", "0/250", "500EXP");
addChallenge(2, "Incinerator IV", "Incinerate 500 Zombies with the Incendinary Rounds Specialization", "0/500", "1000EXP");
addChallenge(2, "Incinerator V", "Incinerate 1000 Zombies with the Incendinary Rounds Specialization", "0/1000", "2500EXP");

addChallenge(2, "Lurker Apprentice", "Reach Affinity Level 15 for this Weapon", "1/15", "250EXP");
addChallenge(2, "Lurker Expert", "Reach Affinity Level 25 for this Weapon", "1/25", "500EXP");
addChallenge(2, "Lurker Mastery", "Reach Affinity Level 35 for this Weapon", "1/35", "750EXP");
addChallenge(2, "Lurker Overlord", "Reach the maximum Affinity Level for this Weapon", "1/50", "0EXP");

//Disposition Challenges
addChallenge(3, "Disposition Marksman I", "Get 15 Kills using the Disposition", "0/15", "25EXP");
addChallenge(3, "Disposition Marksman II", "Get 50 Kills using the Disposition", "0/50", "50EXP");
addChallenge(3, "Disposition Marksman III", "Get 150 Kills using the Disposition", "0/150", "100EXP");
addChallenge(3, "Disposition Marksman IV", "Get 300 Kills using the Disposition", "0/300", "250EXP");
addChallenge(3, "Disposition Marksman V", "Get 750 Kills using the Disposition", "0/750", "500EXP");
addChallenge(3, "Disposition Marksman VI", "Get 1500 Kills using the Disposition", "0/1500", "750EXP");
addChallenge(3, "Disposition Marksman VII", "Get 3000 Kills using the Disposition", "0/3000", "1500EXP");

addChallenge(3, "Disposition Sharpshooter I", "Get 15 Headshot Kills with the Disposition", "0/15", "50EXP");
addChallenge(3, "Disposition Sharpshooter II", "Get 50 Headshot Kills with the Disposition", "0/50", "100EXP");
addChallenge(3, "Disposition Sharpshooter III", "Get 100 Headshot Kills with the Disposition", "0/100", "250EXP");
addChallenge(3, "Disposition Sharpshooter IV", "Get 250 Headshot Kills with the Disposition", "0/250", "500EXP");
addChallenge(3, "Disposition Sharpshooter V", "Get 750 Headshot Kills with the Disposition", "0/750", "1000EXP");
addChallenge(3, "Disposition Sharpshooter VI", "Get 1500 Headshot Kills with the Disposition", "0/1500", "2000EXP");

addChallenge(3, "Disposition Frenzy I", "Kill 2 or more Zombies with a single shot, 5 times", "0/5", "75EXP");
addChallenge(3, "Disposition Frenzy II", "Kill 2 or more Zombies with a single shot, 25 times", "0/25", "250EXP");
addChallenge(3, "Disposition Frenzy III", "Kill 2 or more Zombies with a single shot, 75 times", "0/75", "750EXP");

addChallenge(3, "Incinerator I", "Incinerate 25 Zombies with the Incendinary Rounds Specialization", "0/25", "50EXP");
addChallenge(3, "Incinerator II", "Incinerate 100 Zombies with the Incendinary Rounds Specialization", "0/100", "100EXP");
addChallenge(3, "Incinerator III", "Incinerate 250 Zombies with the Incendinary Rounds Specialization", "0/250", "500EXP");
addChallenge(3, "Incinerator IV", "Incinerate 500 Zombies with the Incendinary Rounds Specialization", "0/500", "1000EXP");
addChallenge(3, "Incinerator V", "Incinerate 1000 Zombies with the Incendinary Rounds Specialization", "0/1000", "2500EXP");

addChallenge(3, "Disposition Apprentice", "Reach Affinity Level 15 for this Weapon", "1/15", "250EXP");
addChallenge(3, "Disposition Expert", "Reach Affinity Level 25 for this Weapon", "1/25", "500EXP");
addChallenge(3, "Disposition Mastery", "Reach Affinity Level 35 for this Weapon", "1/35", "750EXP");
addChallenge(3, "Disposition Overlord", "Reach the maximum Affinity Level for this Weapon", "1/50", "0EXP");

//Swarmer Challenges
addChallenge(4, "Swarmer Marksman I", "Get 15 Kills using the Swarmer", "0/15", "25EXP");
addChallenge(4, "Swarmer Marksman II", "Get 50 Kills using the Swarmer", "0/50", "50EXP");
addChallenge(4, "Swarmer Marksman III", "Get 150 Kills using the Swarmer", "0/150", "100EXP");
addChallenge(4, "Swarmer Marksman IV", "Get 300 Kills using the Swarmer", "0/300", "250EXP");
addChallenge(4, "Swarmer Marksman V", "Get 750 Kills using the Swarmer", "0/750", "500EXP");
addChallenge(4, "Swarmer Marksman VI", "Get 1500 Kills using the Swarmer", "0/1500", "750EXP");
addChallenge(4, "Swarmer Marksman VII", "Get 3000 Kills using the Swarmer", "0/3000", "1500EXP");

addChallenge(4, "Chain Reaction", "Kill 4+ Zombies with the Burst Fire Attack", "0/1", "500EXP");

addChallenge(4, "Incinerator I", "Incinerate 25 Zombies with the Incendinary Rounds Specialization", "0/25", "50EXP");
addChallenge(4, "Incinerator II", "Incinerate 100 Zombies with the Incendinary Rounds Specialization", "0/100", "100EXP");
addChallenge(4, "Incinerator III", "Incinerate 250 Zombies with the Incendinary Rounds Specialization", "0/250", "500EXP");
addChallenge(4, "Incinerator IV", "Incinerate 500 Zombies with the Incendinary Rounds Specialization", "0/500", "1000EXP");
addChallenge(4, "Incinerator V", "Incinerate 1000 Zombies with the Incendinary Rounds Specialization", "0/1000", "2500EXP");

addChallenge(4, "Force Wave I", "Kill 50 Zombies with the Shockwave Specialization", "0/50", "100EXP");
addChallenge(4, "Force Wave II", "Kill 150 Zombies with the Shockwave Specialization", "0/150", "250EXP");
addChallenge(4, "Force Wave III", "Kill 500 Zombies with the Shockwave Specialization", "0/500", "1000EXP");
addChallenge(4, "Force Wave IV", "Kill 1000 Zombies with the Shockwave Specialization", "0/1000", "2000EXP");
addChallenge(4, "Force Wave V", "Kill 2500 Zombies with the Shockwave Specialization", "0/2500", "5000EXP");

addChallenge(4, "Swarmer Apprentice", "Reach Affinity Level 15 for this Weapon", "1/15", "250EXP");
addChallenge(4, "Swarmer Expert", "Reach Affinity Level 25 for this Weapon", "1/25", "500EXP");
addChallenge(4, "Swarmer Mastery", "Reach Affinity Level 35 for this Weapon", "1/35", "750EXP");
addChallenge(4, "Swarmer Overlord", "Reach the maximum Affinity Level for this Weapon", "1/50", "0EXP");

//Rocket Launcher Challenges
addChallenge(5, "Launcher Marksman I", "Get 15 Kills using the Rocket Launcher", "0/15", "25EXP");
addChallenge(5, "Launcher Marksman II", "Get 50 Kills using the Rocket Launcher", "0/50", "50EXP");
addChallenge(5, "Launcher Marksman III", "Get 150 Kills using the Rocket Launcher", "0/150", "100EXP");
addChallenge(5, "Launcher Marksman IV", "Get 300 Kills using the Rocket Launcher", "0/300", "250EXP");
addChallenge(5, "Launcher Marksman V", "Get 750 Kills using the Rocket Launcher", "0/750", "500EXP");
addChallenge(5, "Launcher Marksman VI", "Get 1500 Kills using the Rocket Launcher", "0/1500", "750EXP");
addChallenge(5, "Launcher Marksman VII", "Get 3000 Kills using the Rocket Launcher", "0/3000", "1500EXP");

addChallenge(5, "Force Wave I", "Kill 50 Zombies with the Shockwave Specialization", "0/50", "100EXP");
addChallenge(5, "Force Wave II", "Kill 150 Zombies with the Shockwave Specialization", "0/150", "250EXP");
addChallenge(5, "Force Wave III", "Kill 500 Zombies with the Shockwave Specialization", "0/500", "1000EXP");
addChallenge(5, "Force Wave IV", "Kill 1000 Zombies with the Shockwave Specialization", "0/1000", "2000EXP");
addChallenge(5, "Force Wave V", "Kill 2500 Zombies with the Shockwave Specialization", "0/2500", "5000EXP");

addChallenge(5, "Red Dawn I", "Kill 50 Zombies with the Mini-Nuke Specialization", "0/50", "100EXP");
addChallenge(5, "Red Dawn II", "Kill 250 Zombies with the Mini-Nuke Specialization", "0/250", "250EXP");
addChallenge(5, "Red Dawn III", "Kill 500 Zombies with the Mini-Nuke Specialization", "0/500", "1000EXP");
addChallenge(5, "Red Dawn IV", "Kill 1000 Zombies with the Mini-Nuke Specialization", "0/1000", "2500EXP");
addChallenge(5, "Red Dawn V", "Kill 2500 Zombies with the Mini-Nuke Specialization", "0/2500", "5000EXP");

addChallenge(5, "Rocket Launcher Apprentice", "Reach Affinity Level 15 for this Weapon", "1/15", "250EXP");
addChallenge(5, "Rocket Launcher Expert", "Reach Affinity Level 25 for this Weapon", "1/25", "500EXP");
addChallenge(5, "Rocket Launcher Mastery", "Reach Affinity Level 35 for this Weapon", "1/35", "750EXP");
addChallenge(5, "Rocket Launcher Overlord", "Reach the maximum Affinity Level for this Weapon", "1/50", "0EXP");

//Mamba Challenges
addChallenge(6, "Mamba Marksman I", "Get 15 Kills using the Mamba", "0/15", "25EXP");
addChallenge(6, "Mamba Marksman II", "Get 50 Kills using the Mamba", "0/50", "50EXP");
addChallenge(6, "Mamba Marksman III", "Get 150 Kills using the Mamba", "0/150", "100EXP");
addChallenge(6, "Mamba Marksman IV", "Get 300 Kills using the Mamba", "0/300", "250EXP");
addChallenge(6, "Mamba Marksman V", "Get 750 Kills using the Mamba", "0/750", "500EXP");
addChallenge(6, "Mamba Marksman VI", "Get 1500 Kills using the Mamba", "0/1500", "750EXP");
addChallenge(6, "Mamba Marksman VII", "Get 3000 Kills using the Mamba", "0/3000", "1500EXP");

addChallenge(6, "Mamba Sharpshooter I", "Get 15 Headshot Kills with the Mamba", "0/15", "50EXP");
addChallenge(6, "Mamba Sharpshooter II", "Get 50 Headshot Kills with the Mamba", "0/50", "100EXP");
addChallenge(6, "Mamba Sharpshooter III", "Get 100 Headshot Kills with the Mamba", "0/100", "250EXP");
addChallenge(6, "Mamba Sharpshooter IV", "Get 250 Headshot Kills with the Mamba", "0/250", "500EXP");
addChallenge(6, "Mamba Sharpshooter V", "Get 750 Headshot Kills with the Mamba", "0/750", "1000EXP");
addChallenge(6, "Mamba Sharpshooter VI", "Get 1500 Headshot Kills with the Mamba", "0/1500", "2000EXP");

addChallenge(6, "Incinerator I", "Incinerate 25 Zombies with the Incendinary Rounds Specialization", "0/25", "50EXP");
addChallenge(6, "Incinerator II", "Incinerate 100 Zombies with the Incendinary Rounds Specialization", "0/100", "100EXP");
addChallenge(6, "Incinerator III", "Incinerate 250 Zombies with the Incendinary Rounds Specialization", "0/250", "500EXP");
addChallenge(6, "Incinerator IV", "Incinerate 500 Zombies with the Incendinary Rounds Specialization", "0/500", "1000EXP");
addChallenge(6, "Incinerator V", "Incinerate 1000 Zombies with the Incendinary Rounds Specialization", "0/1000", "2500EXP");

addChallenge(6, "On The Pulse I", "Disintegrate 25 Zombies with the Pulse Rounds Specialization", "0/25", "50EXP");
addChallenge(6, "On The Pulse II", "Disintegrate 100 Zombies with the Pulse Rounds Specialization", "0/100", "100EXP");
addChallenge(6, "On The Pulse III", "Disintegrate 250 Zombies with the Pulse Rounds Specialization", "0/250", "500EXP");
addChallenge(6, "On The Pulse IV", "Disintegrate 500 Zombies with the Pulse Rounds Specialization", "0/500", "1000EXP");
addChallenge(6, "On The Pulse V", "Disintegrate 1000 Zombies with the Pulse Rounds Specialization", "0/1000", "2500EXP");

addChallenge(6, "Mamba Apprentice", "Reach Affinity Level 15 for this Weapon", "1/15", "250EXP");
addChallenge(6, "Mamba Expert", "Reach Affinity Level 25 for this Weapon", "1/25", "500EXP");
addChallenge(6, "Mamba Mastery", "Reach Affinity Level 35 for this Weapon", "1/35", "750EXP");
addChallenge(6, "Mamba Overlord", "Reach the maximum Affinity Level for this Weapon", "1/50", "0EXP");

//Kral Challenges
addChallenge(7, "Kral Marksman I", "Get 15 Kills using the Kral", "0/15", "25EXP");
addChallenge(7, "Kral Marksman II", "Get 50 Kills using the Kral", "0/50", "50EXP");
addChallenge(7, "Kral Marksman III", "Get 150 Kills using the Kral", "0/150", "100EXP");
addChallenge(7, "Kral Marksman IV", "Get 300 Kills using the Kral", "0/300", "250EXP");
addChallenge(7, "Kral Marksman V", "Get 750 Kills using the Kral", "0/750", "500EXP");
addChallenge(7, "Kral Marksman VI", "Get 1500 Kills using the Kral", "0/1500", "750EXP");
addChallenge(7, "Kral Marksman VII", "Get 3000 Kills using the Kral", "0/3000", "1500EXP");
addChallenge(7, "Kral Marksman VIII", "Get 5000 Kills using the Kral", "0/5000", "2000EXP");
addChallenge(7, "Kral Marksman IX", "Get 7500 Kills using the Kral", "0/7500", "2500EXP");

addChallenge(7, "Kral Sharpshooter I", "Get 15 Headshot Kills with the Kral", "0/15", "50EXP");
addChallenge(7, "Kral Sharpshooter II", "Get 50 Headshot Kills with the Kral", "0/50", "100EXP");
addChallenge(7, "Kral Sharpshooter III", "Get 100 Headshot Kills with the Kral", "0/100", "250EXP");
addChallenge(7, "Kral Sharpshooter IV", "Get 250 Headshot Kills with the Kral", "0/250", "500EXP");
addChallenge(7, "Kral Sharpshooter V", "Get 750 Headshot Kills with the Kral", "0/750", "1000EXP");
addChallenge(7, "Kral Sharpshooter VI", "Get 1500 Headshot Kills with the Kral", "0/1500", "2000EXP");

addChallenge(7, "Little Tiny DEATHmare I", "Chain Disintegrate 3+ Zombies using the Shockwave Specialization 15 Times", "0/15", "250EXP");
addChallenge(7, "Little Tiny DEATHmare II", "Chain Disintegrate 3+ Zombies using the Shockwave Specialization 50 Times", "0/50", "750EXP");
addChallenge(7, "Little Tiny DEATHmare III", "Chain Disintegrate 3+ Zombies using the Shockwave Specialization 100 Times", "0/100", "1250EXP");
addChallenge(7, "Little Tiny DEATHmare IV", "Chain Disintegrate 3+ Zombies using the Shockwave Specialization 250 Times", "0/250", "2500EXP");

addChallenge(7, "Kral Apprentice", "Reach Affinity Level 15 for this Weapon", "1/15", "250EXP");
addChallenge(7, "Kral Expert", "Reach Affinity Level 25 for this Weapon", "1/25", "500EXP");
addChallenge(7, "Kral Mastery", "Reach Affinity Level 35 for this Weapon", "1/35", "750EXP");
addChallenge(7, "Kral Overlord", "Reach the maximum Affinity Level for this Weapon", "1/50", "0EXP");

//Kraad Challenges
addChallenge(8, "Kraad Marksman I", "Get 15 Kills using the Kral", "0/15", "25EXP");
addChallenge(8, "Kraad Marksman II", "Get 50 Kills using the Kral", "0/50", "100EXP");
addChallenge(8, "Kraad Marksman III", "Get 150 Kills using the Kral", "0/150", "250EXP");
addChallenge(8, "Kraad Marksman IV", "Get 300 Kills using the Kral", "0/300", "500EXP");
addChallenge(8, "Kraad Marksman V", "Get 750 Kills using the Kral", "0/750", "1000EXP");
addChallenge(8, "Kraad Marksman VI", "Get 1500 Kills using the Kral", "0/1500", "1250EXP");
addChallenge(8, "Kraad Marksman VII", "Get 3000 Kills using the Kral", "0/3000", "2000EXP");
addChallenge(8, "Kraad Marksman VIII", "Get 5000 Kills using the Kral", "0/5000", "5000EXP");
addChallenge(8, "Kraad Marksman IX", "Get 7500 Kills using the Kral", "0/75000", "10000EXP");

addChallenge(8, "Kraad Sharpshooter I", "Get 15 Headshot Kills with the Kraad", "0/15", "50EXP");
addChallenge(8, "Kraad Sharpshooter II", "Get 50 Headshot Kills with the Kraad", "0/50", "100EXP");
addChallenge(8, "Kraad Sharpshooter III", "Get 100 Headshot Kills with the Kraad", "0/100", "250EXP");
addChallenge(8, "Kraad Sharpshooter IV", "Get 250 Headshot Kills with the Kraad", "0/250", "500EXP");
addChallenge(8, "Kraad Sharpshooter V", "Get 750 Headshot Kills with the Kraad", "0/750", "1000EXP");
addChallenge(8, "Kraad Sharpshooter VI", "Get 1500 Headshot Kills with the Kraad", "0/1500", "2000EXP");

addChallenge(8, "Kraad Apprentice", "Reach Affinity Level 15 for this Weapon", "1/15", "250EXP");
addChallenge(8, "Kraad Expert", "Reach Affinity Level 25 for this Weapon", "1/25", "500EXP");
addChallenge(8, "Kraad Mastery", "Reach Affinity Level 35 for this Weapon", "1/35", "750EXP");
addChallenge(8, "Kraad Overlord", "Reach the maximum Affinity Level for this Weapon", "1/50", "0EXP");

//Kralmok Challenges
addChallenge(9, "Kralmok Marksman I", "Get 15 Kills using the Kralmok", "0/15", "25EXP");
addChallenge(9, "Kralmok Marksman II", "Get 50 Kills using the Kralmok", "0/50", "50EXP");
addChallenge(9, "Kralmok Marksman III", "Get 150 Kills using the Kralmok", "0/150", "100EXP");
addChallenge(9, "Kralmok Marksman IV", "Get 300 Kills using the Kralmok", "0/300", "250EXP");
addChallenge(9, "Kralmok Marksman V", "Get 750 Kills using the Kralmok", "0/750", "500EXP");
addChallenge(9, "Kralmok Marksman VI", "Get 1500 Kills using the Kralmok", "0/1500", "750EXP");
addChallenge(9, "Kralmok Marksman VII", "Get 3000 Kills using the Kralmok", "0/3000", "1500EXP");
addChallenge(9, "Kralmok Marksman VIII", "Get 5000 Kills using the Kralmok", "0/5000", "2000EXP");
addChallenge(9, "Kralmok Marksman IX", "Get 7500 Kills using the Kralmok", "0/7500", "5000EXP");

addChallenge(9, "Extreme Measures I", "Chain Disintegrate 5+ Zombies using the Shockwave Specialization 15 Times", "0/15", "250EXP");
addChallenge(9, "Extreme Measures II", "Chain Disintegrate 5+ Zombies using the Shockwave Specialization 50 Times", "0/50", "750EXP");
addChallenge(9, "Extreme Measures III", "Chain Disintegrate 5+ Zombies using the Shockwave Specialization 100 Times", "0/100", "1250EXP");
addChallenge(9, "Extreme Measures IV", "Chain Disintegrate 5+ Zombies using the Shockwave Specialization 250 Times", "0/250", "2500EXP");
addChallenge(9, "Extreme Measures V", "Chain Disintegrate 5+ Zombies using the Shockwave Specialization 500 Times", "0/500", "5000EXP");
addChallenge(9, "Extreme Measures VI", "Chain Disintegrate 5+ Zombies using the Shockwave Specialization 1000 Times", "0/1000", "7500EXP");

addChallenge(9, "Look Ma, Sand!", "Disintegrate 7 Zombies with one shot.", "0/1", "500EXP");

addChallenge(9, "Kralmok Apprentice", "Reach Affinity Level 15 for this Weapon", "1/15", "250EXP");
addChallenge(9, "Kralmok Expert", "Reach Affinity Level 25 for this Weapon", "1/25", "500EXP");
addChallenge(9, "Kralmok Mastery", "Reach Affinity Level 35 for this Weapon", "1/35", "750EXP");
addChallenge(9, "Kralmok Overlord", "Reach the maximum Affinity Level for this Weapon", "1/50", "0EXP");

//Tesla Forcer Challenges
addChallenge(10, "Tesla Forcer Marksman I", "Get 15 Kills using the Tesla Forcer", "0/15", "25EXP");
addChallenge(10, "Tesla Forcer Marksman II", "Get 50 Kills using the Tesla Forcer", "0/50", "50EXP");
addChallenge(10, "Tesla Forcer Marksman III", "Get 150 Kills using the Tesla Forcer", "0/150", "100EXP");
addChallenge(10, "Tesla Forcer Marksman IV", "Get 300 Kills using the Tesla Forcer", "0/300", "250EXP");
addChallenge(10, "Tesla Forcer Marksman V", "Get 750 Kills using the Tesla Forcer", "0/750", "500EXP");
addChallenge(10, "Tesla Forcer Marksman VI", "Get 1500 Kills using the Tesla Forcer", "0/1500", "750EXP");
addChallenge(10, "Tesla Forcer Marksman VII", "Get 3000 Kills using the Tesla Forcer", "0/3000", "1500EXP");
addChallenge(10, "Tesla Forcer Marksman VIII", "Get 5000 Kills using the Tesla Forcer", "0/5000", "2000EXP");
addChallenge(10, "Tesla Forcer Marksman IX", "Get 7500 Kills using the Tesla Forcer", "0/7500", "5000EXP");

addChallenge(10, "Call me Nikolai!", "Using the Ion Storm Specialization, Wipe out 15 Zombies with one shot.", "0/1", "500EXP");

addChallenge(10, "Tesla Forcer Apprentice", "Reach Affinity Level 15 for this Weapon", "1/15", "250EXP");
addChallenge(10, "Tesla Forcer Expert", "Reach Affinity Level 25 for this Weapon", "1/25", "500EXP");
addChallenge(10, "Tesla Forcer Mastery", "Reach Affinity Level 35 for this Weapon", "1/35", "750EXP");
addChallenge(10, "Tesla Forcer Overlord", "Reach the maximum Affinity Level for this Weapon", "1/50", "0EXP");

//returns a tabulated string of ID's
function getMarksmanChallenges(%weaponID) {
   for(%i = 0; getChallenge(%weaponID, %i) !$= "ERROR"; %i++) {
      %c = getChallenge(%weaponID, %i);
      if(strstr(getField(%c, 0), "Marksman") != -1) {
         if(%idOut $= "") {
            %idOut = %i;
         }
         else {
            %idOut = %idOut @ "\t" @ %i;
         }
      }
   }
   return %idOut;
}

//returns a tabulated string of ID's
function getSharpshooterChallenges(%weaponID) {
   for(%i = 0; getChallenge(%weaponID, %i) !$= "ERROR"; %i++) {
      %c = getChallenge(%weaponID, %i);
      if(strstr(getField(%c, 0), "Sharpshooter") != -1) {
         if(%idOut $= "") {
            %idOut = %i;
         }
         else {
            %idOut = %idOut @ "\t" @ %i;
         }
      }
   }
   return %idOut;
}

//returns a tabulated string of ID's
function getLevelChallenges(%weaponID) {
   for(%i = 0; getChallenge(%weaponID, %i) !$= "ERROR"; %i++) {
      %c = getChallenge(%weaponID, %i);
      if(strstr(getField(%c, 0), "Apprentice") != -1
      || strstr(getField(%c, 0), "Expert") != -1
      || strstr(getField(%c, 0), "Mastery") != -1
      || strstr(getField(%c, 0), "Overlord") != -1) {
         if(%idOut $= "") {
            %idOut = %i;
         }
         else {
            %idOut = %idOut @ "\t" @ %i;
         }
      }
   }
   return %idOut;
}
