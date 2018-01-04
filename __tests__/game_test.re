open Jest;
include List;
include Array;
open Deck;
open Game;

describe("Bid", () => {
  open ExpectJs;
  open Game.Bid;

  let isValidBid = validBid([Bid(North, 80, Deck.Spades)]);
  let multiEqBuilder = (valueMap, expectedMap) => lst => lst 
    |> List.map(valueMap) |> Array.of_list 
    |> expect |> toEqual(
      lst |> List.map(expectedMap) |> Array.of_list
    );
  let multiEq = multiEqBuilder(((value, _)) => value, ((_, expected)) => expected);
  let multiEqFalse = multiEqBuilder(v => v, (_) => false);
  let multiEqTrue = multiEqBuilder(v => v, (_) => true);

  test("Pass is valid when next", () => {
    Pass(East) |> validBid([Pass(North)]) |> expect |> toEqual(true);
  });
  test("Pass is invalid when not next", () => {
    Pass(South) |> validBid([Pass(North)]) |> expect |> toEqual(true);
  });
  test("A valid bid specials", () => {
    [
      Bid(East, 80, Deck.Spades) |> validBid([]),
      Bid(East, 100, Deck.Spades) |> validBid([]),
      Pass(East) |> isValidBid,
    ] |> multiEqTrue;
  });
  test("A valid bid values", () => {
    [90, 100, 160, 250, 400] 
      |> List.map(v => Bid(East, v, Deck.Spades)) 
      |> List.map(isValidBid) 
      |> multiEqTrue;
  });
  test("An invalid bid", () => {
    [70, 91, 170, 260, 800] 
      |> List.map(v => Bid(East, v, Deck.Spades)) 
      |> List.map(isValidBid) 
      |> multiEqFalse;
  });
});

describe("Game", () => {
  open ExpectJs;
  open Game.Bid;

  let mkBid = (p, v) => Bid(p, v, Deck.Diamonds);
  let dispatchBid = (p, v) => Game.MakeBid(mkBid(p, v)) |> Game.dispatch;

  test("We can make a bid", () => {
    let state = Game.initialState() |> dispatchBid(East, 80);
    state.bids |> List.length |> expect |> toEqual(1);
  });

  test("We can make multiple bids", () => {
    let state = Game.initialState() 
      |> dispatchBid(North, 80)
      |> dispatchBid(East, 90)
      |> dispatchBid(South, 100)
      |> dispatchBid(West, 110)
      ;
    state.bids |> List.length |> expect |> toEqual(4);
  });

  test("Bad bids gets rejected", () => {
    let state = Game.initialState() 
      |> dispatchBid(North, 80)
      |> dispatchBid(East, 80)
      ;
    state.bids |> List.length |> expect |> toEqual(1);
  });

  test("Bad bids gets rejected", () => {
    let state = Game.initialState() 
      |> dispatchBid(North, 80)
      |> dispatchBid(East, 80)
      ;
    state.error |> expect |> toEqual(Some(Game.InvalidBid(mkBid(East, 80))));
  });
});